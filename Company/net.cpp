/*	EQEMu: Everquest Server Emulator
Copyright (C) 2001-2002 EQEMu Development Team (http://eqemu.org)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY except by those people which sell it, which
are required to give you total support for your newly bought product;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/
#include "../common/global_define.h"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../common/string_util.h"
#include "../common/eqemu_logsys.h"
#include "../common/queue.h"
#include "../common/timer.h"
#include "../common/eq_packet.h"
#include "../common/seperator.h"
#include "../common/version.h"
#include "../common/eqtime.h"
#include "../common/event/event_loop.h"
#include "../common/net/eqstream.h"
#include "../common/opcodemgr.h"
#include "../common/guilds.h"
#include "../common/eq_stream_ident.h"
#include "../common/rulesys.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "client.h"
#include "worlddb.h"
#ifdef _WINDOWS
#include <process.h>
#define snprintf	_snprintf
#define strncasecmp	_strnicmp
#define strcasecmp	_stricmp
#include <conio.h>
#else
#include <pthread.h>
#include "../common/unix.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#if not defined (FREEBSD) && not defined (DARWIN)
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
	struct seminfo *__buf;
	void *__pad;
};
#endif

#endif

#include "../common/patches/patches.h"
#include "../common/random.h"
#include "zoneserver.h"
#include "login_server.h"
#include "login_server_list.h"
#include "world_config.h"
#include "zoneserver.h"
#include "zonelist.h"
#include "clientlist.h"
#include "web_interface.h"
#include "console.h"

#include "../common/net/servertalk_server.h"

ClientList client_list;
ZSList zoneserver_list;
LoginServerList loginserverlist;
EQEmu::Random emu_random;
volatile bool RunLoops = true;
uint32 numclients = 0;
uint32 numzones = 0;
bool holdzones = false;
const WorldConfig *Config;
EQEmuLogSys LogSys;
WebInterfaceList web_interface;

void CatchSignal(int sig_num);

inline void UpdateWindowTitle(std::string new_title) {
#ifdef _WINDOWS
	SetConsoleTitle(new_title.c_str());
#endif
}

int main(int argc, char** argv) {
	RegisterExecutablePlatform(ExePlatformWorld);
	LogSys.LoadLogSettingsDefaults();
	set_exception_handler();

	/* Database Version Check */
	uint32 Database_Version = CURRENT_BINARY_DATABASE_VERSION;
	uint32 Bots_Database_Version = CURRENT_BINARY_BOTS_DATABASE_VERSION;
	if (argc >= 2) {
		if (strcasecmp(argv[1], "db_version") == 0) {
			std::cout << "Binary Database Version: " << Database_Version << " : " << Bots_Database_Version << std::endl;
			return 0;
		}
	}

	// Load server configuration
	Log(Logs::General, Logs::World_Server, "Loading server configuration..");
	if (!WorldConfig::LoadConfig()) {
		Log(Logs::General, Logs::World_Server, "Loading server configuration failed.");
		return 1;
	}

	Config = WorldConfig::get();

	Log(Logs::General, Logs::World_Server, "CURRENT_VERSION: %s", CURRENT_VERSION);

	if (signal(SIGINT, CatchSignal) == SIG_ERR) {
		Log(Logs::General, Logs::World_Server, "Could not set signal handler");
		return 1;
	}

	if (signal(SIGTERM, CatchSignal) == SIG_ERR) {
		Log(Logs::General, Logs::World_Server, "Could not set signal handler");
		return 1;
	}

#ifndef WIN32
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		Log(Logs::General, Logs::World_Server, "Could not set signal handler");
		return 1;
	}
#endif

	// add login server config to list
	if (Config->LoginCount == 0) {
		if (Config->LoginHost.length()) {
			loginserverlist.Add(Config->LoginHost.c_str(), Config->LoginPort, Config->LoginAccount.c_str(), Config->LoginPassword.c_str(), Config->LoginLegacy);
			Log(Logs::General, Logs::World_Server, "Added loginserver %s:%i", Config->LoginHost.c_str(), Config->LoginPort);
		}
	}
	else {
		LinkedList<LoginConfig*> loginlist = Config->loginlist;
		LinkedListIterator<LoginConfig*> iterator(loginlist);
		iterator.Reset();
		while (iterator.MoreElements()) {
			loginserverlist.Add(iterator.GetData()->LoginHost.c_str(), iterator.GetData()->LoginPort, iterator.GetData()->LoginAccount.c_str(), iterator.GetData()->LoginPassword.c_str(),
				iterator.GetData()->LoginLegacy);
			Log(Logs::General, Logs::World_Server, "Added loginserver %s:%i", iterator.GetData()->LoginHost.c_str(), iterator.GetData()->LoginPort);
			iterator.Advance();
		}
	}

	Log(Logs::General, Logs::World_Server, "Connecting to MySQL...");
	if (!database.Connect(
		Config->DatabaseHost.c_str(),
		Config->DatabaseUsername.c_str(),
		Config->DatabasePassword.c_str(),
		Config->DatabaseDB.c_str(),
		Config->DatabasePort)) {
		Log(Logs::General, Logs::World_Server, "Cannot continue without a database connection.");
		return 1;
	}
	/* Register Log System and Settings */
	database.LoadLogSettings(LogSys.log_settings);
	LogSys.StartFileLogs();

	std::string hotfix_name;
	if (database.GetVariable("hotfix_name", hotfix_name)) {
		if (!hotfix_name.empty()) {
			Log(Logs::General, Logs::Zone_Server, "Current hotfix in use: '%s'", hotfix_name.c_str());
		}
	}

	RuleManager::Instance()->SaveRules(&database);

	Log(Logs::General, Logs::World_Server, "Loading EQ time of day..");
	TimeOfDay_Struct eqTime;
	time_t realtime;
	eqTime = database.LoadTime(realtime);
	zoneserver_list.worldclock.SetCurrentEQTimeOfDay(eqTime, realtime);
	Timer EQTimeTimer(600000);
	EQTimeTimer.Start(600000);


	std::string tmp;
	database.GetVariable("holdzones", tmp);
	if (tmp.length() == 1 && tmp[0] == '1') {
		holdzones = true;
	}
	Log(Logs::General, Logs::World_Server, "Reboot zone modes %s", holdzones ? "ON" : "OFF");

	Log(Logs::General, Logs::World_Server, "Deleted %i stale player corpses from database", database.DeleteStalePlayerCorpses());

	Log(Logs::General, Logs::World_Server, "Purging expired instances");
	database.PurgeExpiredInstances();
	Timer PurgeInstanceTimer(450000);
	PurgeInstanceTimer.Start(450000);

	Log(Logs::General, Logs::World_Server, "Loading char create info...");
	database.LoadCharacterCreateAllocations();
	database.LoadCharacterCreateCombos();

	std::unique_ptr<EQ::Net::ConsoleServer> console;
	if (Config->TelnetEnabled) {
		Log(Logs::General, Logs::World_Server, "Console (TCP) listener started.");
		console.reset(new EQ::Net::ConsoleServer(Config->TelnetIP, Config->TelnetTCPPort));
		RegisterConsoleFunctions(console);
	}

	std::unique_ptr<EQ::Net::ServertalkServer> server_connection;
	server_connection.reset(new EQ::Net::ServertalkServer());

	EQ::Net::ServertalkServerOptions server_opts;
	server_opts.port = Config->WorldTCPPort;
	server_opts.ipv6 = false;
	server_opts.credentials = Config->SharedKey;
	server_connection->Listen(server_opts);
	Log(Logs::General, Logs::World_Server, "Server (TCP) listener started.");

	server_connection->OnConnectionIdentified("Zone", [&console](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
		LogF(Logs::General, Logs::World_Server, "New Zone Server connection from {2} at {0}:{1}",
			connection->Handle()->RemoteIP(), connection->Handle()->RemotePort(), connection->GetUUID());

		numzones++;
		zoneserver_list.Add(new ZoneServer(connection, console.get()));
	});

	server_connection->OnConnectionRemoved("Zone", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
		LogF(Logs::General, Logs::World_Server, "Removed Zone Server connection from {0}",
			connection->GetUUID());

		numzones--;
		zoneserver_list.Remove(connection->GetUUID());
	});

	server_connection->OnConnectionIdentified("WebInterface", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
		LogF(Logs::General, Logs::World_Server, "New WebInterface Server connection from {2} at {0}:{1}",
			connection->Handle()->RemoteIP(), connection->Handle()->RemotePort(), connection->GetUUID());

		web_interface.AddConnection(connection);
	});

	server_connection->OnConnectionRemoved("WebInterface", [](std::shared_ptr<EQ::Net::ServertalkServerConnection> connection) {
		LogF(Logs::General, Logs::World_Server, "Removed WebInterface Server connection from {0}",
			connection->GetUUID());

		web_interface.RemoveConnection(connection);
	});

	EQ::Net::EQStreamManagerOptions opts(9000, false, false);
	EQ::Net::EQStreamManager eqsm(opts);

	//register all the patches we have avaliable with the stream identifier.
	EQStreamIdentifier stream_identifier;
	RegisterAllPatches(stream_identifier);
	zoneserver_list.shutdowntimer = new Timer(60000);
	zoneserver_list.shutdowntimer->Disable();
	zoneserver_list.reminder = new Timer(20000);
	zoneserver_list.reminder->Disable();
	Timer InterserverTimer(INTERSERVER_TIMER); // does MySQL pings and auto-reconnect
	InterserverTimer.Trigger();
	EQStreamInterface *eqsi;

	eqsm.OnNewConnection([&stream_identifier](std::shared_ptr<EQ::Net::EQStream> stream) {
		stream_identifier.AddStream(stream);
        LogF(Logs::Detail, Logs::World_Server, "New connection from IP {0}:{1}", stream->RemoteEndpoint(), ntohs(stream->GetRemotePort()));
	});

	while (RunLoops) {
		Timer::SetCurrentTime();

		//give the stream identifier a chance to do its work....
		stream_identifier.Process();

		//check the stream identifier for any now-identified streams
		while ((eqsi = stream_identifier.PopIdentified())) {
			//now that we know what patch they are running, start up their client object
			struct in_addr	in;
			in.s_addr = eqsi->GetRemoteIP();
			if (RuleB(World, UseBannedIPsTable)) { //Lieka: Check to see if we have the responsibility for blocking IPs.
				Log(Logs::Detail, Logs::World_Server, "Checking inbound connection %s against BannedIPs table", inet_ntoa(in));
				if (!database.CheckBannedIPs(inet_ntoa(in))) { //Lieka: Check inbound IP against banned IP table.
					Log(Logs::Detail, Logs::World_Server, "Connection %s PASSED banned IPs check. Processing connection.", inet_ntoa(in));
					auto client = new Client(eqsi);
					// @merth: client->zoneattempt=0;
					client_list.Add(client);
				}
				else {
					Log(Logs::General, Logs::World_Server, "Connection from %s FAILED banned IPs check. Closing connection.", inet_ntoa(in));
					eqsi->Close(); //Lieka: If the inbound IP is on the banned table, close the EQStream.
				}
			}
			if (!RuleB(World, UseBannedIPsTable)) {
				Log(Logs::Detail, Logs::World_Server, "New connection from %s:%d, processing connection", inet_ntoa(in), ntohs(eqsi->GetRemotePort()));
				auto client = new Client(eqsi);
				// @merth: client->zoneattempt=0;
				client_list.Add(client);
			}
		}

		client_list.Process();

		if (PurgeInstanceTimer.Check())
		{
			database.PurgeExpiredInstances();
		}

		if (EQTimeTimer.Check()) {
			TimeOfDay_Struct tod;
			zoneserver_list.worldclock.GetCurrentEQTimeOfDay(time(0), &tod);
			if (!database.SaveTime(tod.minute, tod.hour, tod.day, tod.month, tod.year))
				Log(Logs::General, Logs::World_Server, "Failed to save eqtime.");
			else
				Log(Logs::Detail, Logs::World_Server, "EQTime successfully saved.");
		}

		zoneserver_list.Process();

		if (InterserverTimer.Check()) {
			InterserverTimer.Start();
			database.ping();

			std::string window_title = StringFormat("World: %s Clients: %i", Config->LongName.c_str(), client_list.GetClientCount());
			UpdateWindowTitle(window_title);
		}

		EQ::EventLoop::Get().Process();
		Sleep(5);
	}
	Log(Logs::General, Logs::World_Server, "World main loop completed.");
	Log(Logs::General, Logs::World_Server, "Shutting down zone connections (if any).");
	zoneserver_list.KillAll();
	Log(Logs::General, Logs::World_Server, "Zone (TCP) listener stopped.");
	Log(Logs::General, Logs::World_Server, "Signaling HTTP service to stop...");
	LogSys.CloseFileLogs();

	return 0;
}

void CatchSignal(int sig_num) {
	Log(Logs::General, Logs::World_Server, "Caught signal %d", sig_num);
	RunLoops = false;
}
