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
#include "client.h"
#include "login_server.h"
#include "../common/misc_functions.h"
#include "../common/eqemu_logsys.h"

extern LoginServer server;

Client::Client(std::shared_ptr<EQStreamInterface> c, ClientManager* m)
{
	connection = c;
	status = cs_not_sent_session_ready;
    static unsigned int s_accoutId = 1000;
    account_id = s_accoutId++;
	play_server_id = 0;
	play_sequence_id = 0;
    _clientManager = m;
}

bool Client::Process()
{
	EQApplicationPacket *app = connection->PopPacket();
	while (app)
	{
		if (server.options.IsTraceOn())
		{
			Log(Logs::General, Logs::Login_Server, "Application packet received from client (size %u)", app->Size());
		}

		if (server.options.IsDumpInPacketsOn())
		{
			DumpPacket(app);
		}

		switch (app->GetOpcode())
		{
		case OP_SessionReady:
		{
			if (server.options.IsTraceOn())
			{
				Log(Logs::General, Logs::Login_Server, "Session ready received from client.");
			}
			Handle_SessionReady((const char*)app->pBuffer, app->Size());
			break;
		}
		case OP_Login:
		{
			if (app->Size() < 20)
			{
				Log(Logs::General, Logs::Error, "Login received but it is too small, discarding.");
				break;
			}

			if (server.options.IsTraceOn())
			{
				Log(Logs::General, Logs::Login_Server, "Login received from client.");
			}

			Handle_Login((const char*)app->pBuffer, app->Size());
			break;
		}
		case OP_ServerListRequest:
		{
			if (app->Size() < 4) {
				Log(Logs::General, Logs::Error, "Server List Request received but it is too small, discarding.");
				break;
			}

			if (server.options.IsTraceOn())
			{
				Log(Logs::General, Logs::Login_Server, "Server list request received from client.");
			}

			SendServerListPacket(*(uint32_t*)app->pBuffer);
			break;
		}
		case OP_PlayEverquestRequest:
		{
			if (app->Size() < sizeof(PlayEverquestRequest_Struct))
			{
				Log(Logs::General, Logs::Error, "Play received but it is too small, discarding.");
				break;
			}

			Handle_Play((const char*)app->pBuffer);
			break;
		}
        case OP_moshouEnterWorld:
            Handle_enteredWord((const char*)app->pBuffer, app->Size());
            break;
        case OP_moshUpdateWorld:
            Handle_updateWord((const char*)app->pBuffer, app->Size());
            break;
		default:
		{
			if (LogSys.log_settings[Logs::Client_Server_Packet_Unhandled].is_category_enabled == 1) {
				char dump[64];
				app->build_header_dump(dump);
				Log(Logs::General, Logs::Error, "Recieved unhandled application packet from the client: %s.", dump);
			}
		}
		}

		delete app;
		app = connection->PopPacket();
	}

	return true;
}

void Client::Handle_SessionReady(const char* data, unsigned int size)
{
	if (status != cs_not_sent_session_ready)
	{
		Log(Logs::General, Logs::Error, "Session ready received again after already being received.");
		return;
	}

	if (size < sizeof(unsigned int))
	{
		Log(Logs::General, Logs::Error, "Session ready was too small.");
		return;
	}

	status = cs_waiting_for_login;

	/**
	* The packets are mostly the same but slightly different between the two versions.
	*/
    const char *msg = "ChatMessage";
    EQApplicationPacket *outapp = new EQApplicationPacket(OP_ChatMessage, 16 + strlen(msg));
    outapp->pBuffer[0] = 0x02;
    outapp->pBuffer[10] = 0x01;
    outapp->pBuffer[11] = 0x65;
    strcpy((char*)(outapp->pBuffer + 15), msg);

    if (server.options.IsDumpOutPacketsOn())
    {
        DumpPacket(outapp);
    }

    connection->QueuePacket(outapp);
    delete outapp;

}

void Client::Handle_enteredWord(const char *data, unsigned int size)
{
    const MS_ClientInfo* enter = (MS_ClientInfo*)data;
    if(status == cs_enteredWord)
    {
        Log(Logs::General, Logs::Error, "already entered world.");
        return;
    }
    else
    {
        status = cs_enteredWord;
    }
    memcpy(&clientInfor, enter, sizeof(MS_ClientInfo));
    _clientManager->ProcessClientEnterWorld(this);
}

void Client::Handle_updateWord(const char *data, unsigned int size)
{
    const MS_ClientInfo* enter = (MS_ClientInfo*)data;
    memcpy(&clientInfor, enter, sizeof(MS_ClientInfo));
    _clientManager->ProcessClientUpdate(this);
}

void Client::Handle_Login(const char* data, unsigned int size)
{
	auto mode = server.options.GetEncryptionMode();

    if (status != cs_waiting_for_login && status != cs_failed_to_login) {
		Log(Logs::General, Logs::Error, "Login received after already having logged in.");
		return;
	}

	if ((size - 12) % 8 != 0) {
		Log(Logs::General, Logs::Error, "Login received packet of size: %u, this would cause a block corruption, discarding.", size);
		return;
	}

	char *login_packet_buffer = nullptr;

	unsigned int db_account_id = 0;
	std::string db_account_password_hash;

	std::string outbuffer;
	outbuffer.resize(size - 12);
	if (outbuffer.length() == 0) {
		LogF(Logs::General, Logs::Debug, "Corrupt buffer sent to server, no length.");
		return;
	}

    const char* r = eqcrypt_block(data + 10, size - 12, &outbuffer[0], false);
	if (r == nullptr) {
		LogF(Logs::General, Logs::Debug, "Failed to decrypt eqcrypt block");
		return;
	}

	std::string cred;

	std::string user(&outbuffer[0]);
	if (user.length() >= outbuffer.length()) {
		LogF(Logs::General, Logs::Debug, "Corrupt buffer sent to server, preventing buffer overflow.");
		return;
	}

	bool result = false;
	if (outbuffer[0] == 0 && outbuffer[1] == 0) {
		if (server.options.IsTokenLoginAllowed()) {
			cred = (&outbuffer[2 + user.length()]);
			result = server.db->GetLoginTokenDataFromToken(cred, connection->GetRemoteAddr(), db_account_id, user);
		}
	}
	else {
		if (server.options.IsPasswordLoginAllowed()) {
			cred = (&outbuffer[1 + user.length()]);
			if (server.db->GetLoginDataFromAccountName(user, db_account_password_hash, db_account_id) == false) {
				/* If we have auto_create_accounts enabled in the login.ini, we will process the creation of an account on our own*/
				if (server.options.CanAutoCreateAccounts() &&
                    server.db->CreateLoginData(user, eqcrypt_hash(user, cred, mode), db_account_id) == true)
                {
					LogF(Logs::General, Logs::Error, "User {0} does not exist in the database, so we created it...", user);
					result = true;
				}
				else {
					LogF(Logs::General, Logs::Error, "Error logging in, user {0} does not exist in the database.", user);
					result = false;
				}
			}
			else {
				if (eqcrypt_verify_hash(user, cred, db_account_password_hash, mode)) {
					result = true;
				}
				else {
					result = false;
				}
			}
		}
	}

	/* Login Accepted */
	if (result) {

		server.client_manager->RemoveExistingClient(db_account_id);

		in_addr in;
		in.s_addr = connection->GetRemoteIP();

		server.db->UpdateLSAccountData(db_account_id, std::string(inet_ntoa(in)));
		GenerateKey();

		account_id = db_account_id;
		account_name = user;

		EQApplicationPacket *outapp = new EQApplicationPacket(OP_LoginAccepted, 10 + 80);
		const LoginLoginRequest_Struct* llrs = (const LoginLoginRequest_Struct *)data;
		LoginAccepted_Struct* login_accepted = (LoginAccepted_Struct *)outapp->pBuffer;

		LoginFailedAttempts_Struct * login_failed_attempts = new LoginFailedAttempts_Struct;
		memset(login_failed_attempts, 0, sizeof(LoginFailedAttempts_Struct));

		memcpy(login_failed_attempts->key, key.c_str(), key.size());

		char encrypted_buffer[80] = { 0 };
        auto rc = eqcrypt_block((const char*)login_failed_attempts, 75, encrypted_buffer, true);
		if (rc == nullptr) {
			LogF(Logs::General, Logs::Debug, "Failed to encrypt eqcrypt block");
		}

		memcpy(login_accepted->encrypt, encrypted_buffer, 80);

		if (server.options.IsDumpOutPacketsOn()) {
			DumpPacket(outapp);
		}

		connection->QueuePacket(outapp);
		delete outapp;

		status = cs_logged_in;
    }else
    {
        EQApplicationPacket *outapp = new EQApplicationPacket(OP_LoginFailed, sizeof(LoginLoginFailed_Struct));
		LoginLoginFailed_Struct* llas = (LoginLoginFailed_Struct *)outapp->pBuffer;
        memcpy(llas->message, FailedLoginResponseData, sizeof(FailedLoginResponseData));

		if (server.options.IsDumpOutPacketsOn()) {
			DumpPacket(outapp);
		}

		connection->QueuePacket(outapp);
		delete outapp;

        status = cs_failed_to_login;
	}
}

void Client::Handle_Play(const char* data)
{
	if (status != cs_logged_in)
	{
		Log(Logs::General, Logs::Error, "Client sent a play request when they were not logged in, discarding.");
		return;
	}

	const PlayEverquestRequest_Struct *play = (const PlayEverquestRequest_Struct*)data;
    unsigned int server_id_in = play->ServerNumber;
    unsigned int sequence_in = play->Sequence;

	if (server.options.IsTraceOn())
	{
		Log(Logs::General, Logs::Login_Server, "Play received from client, server number %u sequence %u.", server_id_in, sequence_in);
	}

    this->play_server_id = play->ServerNumber;
	play_sequence_id = sequence_in;
	play_server_id = server_id_in;
	server.server_manager->SendUserToWorldRequest(server_id_in, account_id);
}

void Client::SendServerListPacket(uint32 seq)
{
	EQApplicationPacket *outapp = server.server_manager->CreateServerListPacket(this, seq);

	if (server.options.IsDumpOutPacketsOn())
	{
		DumpPacket(outapp);
	}

	connection->QueuePacket(outapp);
	delete outapp;
}

void Client::SendPlayResponse(EQApplicationPacket *outapp)
{
	if (server.options.IsTraceOn())
	{
		Log(Logs::General, Logs::Netcode, "Sending play response for %s.", GetAccountName().c_str());
		// server_log->LogPacket(log_network_trace, (const char*)outapp->pBuffer, outapp->size);
	}
	connection->QueuePacket(outapp);
}

void Client::GenerateKey()
{
	key.clear();
	int count = 0;
	while (count < 10)
	{
		static const char key_selection[] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9'
		};

		key.append((const char*)&key_selection[random.Int(0, 35)], 1);
		count++;
    }
}

void Client::processAClientEnter(Client *other)
{
    if(status == cs_enteredWord)
    {
        EQApplicationPacket packet(OP_moshouAddClient, sizeof (MS_EnterWorldData));
        MS_EnterWorldData data;
        data.accountId = other->account_id;
        data.clientInfo = other->clientInfor;
        packet.WriteData(&data, sizeof (MS_EnterWorldData));
        connection->QueuePacket(&packet, true);
    }
}

void Client::processClientUpdate(Client *other)
{
    if(status == cs_enteredWord)
    {
        EQApplicationPacket packet(OP_moshUpdateWorld, sizeof (MS_EnterWorldData));
        MS_EnterWorldData data;
        data.accountId = other->account_id;
        data.clientInfo = other->clientInfor;
        packet.WriteData(&data, sizeof (MS_EnterWorldData));
        connection->QueuePacket(&packet, false);
    }
}

void Client::loadOtherClient(Client *other)
{
    if(other->status == cs_enteredWord)
    {
        EQApplicationPacket packet(OP_moshouAddClient, sizeof (MS_EnterWorldData));
        MS_EnterWorldData data;
        data.accountId = other->account_id;
        data.clientInfo = other->clientInfor;
        packet.WriteData(&data, sizeof (MS_EnterWorldData));
        connection->QueuePacket(&packet, true);
    }
}
