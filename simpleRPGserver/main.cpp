/*	EQEMu: Everquest Server Emulator
Copyright (C) 2001-2010 EQEMu Development Team (http://eqemulator.net)

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
#include "../common/types.h"
#include "../common/opcodemgr.h"
#include "../common/event/event_loop.h"
#include "../common/timer.h"
#include "../common/platform.h"
#include "../common/crash.h"
#include "../common/eqemu_logsys.h"
#include "login_server.h"
#include <time.h>
#include <stdlib.h>
#include <string>
#include <sstream>

LoginServer server;
EQEmuLogSys LogSys;
bool run_server = true;

void CatchSignal(int sig_num)
{
}

int main()
{
	RegisterExecutablePlatform(ExePlatformLogin);
	set_exception_handler();
	LogSys.LoadLogSettingsDefaults();

	LogSys.log_settings[Logs::Error].log_to_console = Logs::General;
	LogSys.log_settings[Logs::Error].is_category_enabled = 1;

	Log(Logs::General, Logs::Login_Server, "Logging System Init.");

	/* Parse out login.ini */
	server.config = new Config();
    server.config->Parse("login.ini");

	//create our server manager.
	Log(Logs::General, Logs::Login_Server, "Server Manager Initialize.");
	server.server_manager = new ServerManager();
	if (!server.server_manager) {
		//We can't run without a server manager, cleanup and exit.
		Log(Logs::General, Logs::Error, "Server Manager Failed to Start.");

		Log(Logs::General, Logs::Login_Server, "Database System Shutdown.");
		delete server.db;
		Log(Logs::General, Logs::Login_Server, "Config System Shutdown.");
		delete server.config;
		return 1;
	}

	//create our client manager.
	Log(Logs::General, Logs::Login_Server, "Client Manager Initialize.");
	server.client_manager = new ClientManager();
	if (!server.client_manager) {
		//We can't run without a client manager, cleanup and exit.
		Log(Logs::General, Logs::Error, "Client Manager Failed to Start.");
		Log(Logs::General, Logs::Login_Server, "Server Manager Shutdown.");
		delete server.server_manager;

		Log(Logs::General, Logs::Login_Server, "Database System Shutdown.");
		delete server.db;
		Log(Logs::General, Logs::Login_Server, "Config System Shutdown.");
		delete server.config;
		return 1;
	}

	Log(Logs::General, Logs::Login_Server, "Server Started.");
	while (run_server) {
		Timer::SetCurrentTime();
		server.client_manager->Process();
		EQ::EventLoop::Get().Process();
		Sleep(5);
	}

	Log(Logs::General, Logs::Login_Server, "Server Shutdown.");
	Log(Logs::General, Logs::Login_Server, "Client Manager Shutdown.");
	delete server.client_manager;
	Log(Logs::General, Logs::Login_Server, "Server Manager Shutdown.");
	delete server.server_manager;

	Log(Logs::General, Logs::Login_Server, "Database System Shutdown.");
	delete server.db;
	Log(Logs::General, Logs::Login_Server, "Config System Shutdown.");
	delete server.config;
	return 0;
}
