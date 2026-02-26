#pragma once

#ifdef VOICECLIENT_EXPORTS
#define VOICE_API __declspec(dllexport)
#else
#define VOICE_API __declspec(dllimport)
#endif

extern "C" {
	VOICE_API bool InitVoiceClient(const char* username, const char* server_ip, int tcp_port, const char* room, const char* password);
	VOICE_API void ShutdownVoiceClient();
	VOICE_API void RequestConnectedUsers();
}
