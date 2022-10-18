#include "agora.h"


bool AgoraRTCObserver::onSendAudioPacket(Packet& packet)
{
	return true;
}

bool AgoraRTCObserver::onSendVideoPacket(Packet& packet)
{
	return true;
}

bool AgoraRTCObserver::onReceiveAudioPacket(Packet& packet)
{
	print_line(String("ON audio received"));
	print_line(String("Packet size: ") + String(itos(packet.size)));
	return true;
}

bool AgoraRTCObserver::onReceiveVideoPacket(Packet& packet)
{
	return true;
}


void AgoraClient::init(Ref<AudioClientConfig> p_config) {
	if (initialized) { return; }
	AudioClient::init(p_config);
    rtc_engine = (IRtcEngine *)createAgoraRtcEngine();
    event_handler = new RTCEventHandler();
	event_handler->client = this;
    RtcEngineContext engine_context;
    engine_context.eventHandler = event_handler;
	engine_context.appId = p_config->get_app_id().ascii().get_data();
    int err = rtc_engine->initialize(engine_context);
    err = rtc_engine->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
	WARN_PRINT("Agora version:" + String(rtc_engine->getVersion(0)));
	packet_observer.client = this;
	// int value = rtc_engine->registerLocalUserAccount(engine_context.appId, p_config->get_username().ascii().get_data());

	// WARN_PRINT("Register user:" + String(itos(value)));

	// rtc_engine->sendStreamMessage

    initialized = true;
};



// void AgoraClient::set_app_id(const String& p_app_id) {
// 	app_id = p_app_id;
// }

// void Agora::join_channel(const String& channel_id, uid_t user_id, const String& token){

// 	ChannelMediaOptions options;
// 	// For a voice call scenario, set the channel profile as BROADCASTING.
// 	options.channelProfile = CHANNEL_PROFILE_LIVE_BROADCASTING;

// 	// Shut down the camera
// 	options.publishCameraTrack = false;
// 	// Set both clients as the BROADCASTER.
// 	options.clientRoleType = CLIENT_ROLE_BROADCASTER;

// 	string token_str = token.utf8().ptr();
// 	string channel_str = channel_id.utf8().ptr();
// 	int ret = rtc_engine->joinChannel(token_str.c_str(), channel_str.c_str(), user_id, options);
// 	printf("join channel return %i\n", ret);
// }

// void AgoraClient::join_channel(const String& channel_id, const String& token) {

// 	ChannelMediaOptions options;
// 	// For a voice call scenario, set the channel profile as BROADCASTING.
// 	options.channelProfile = CHANNEL_PROFILE_LIVE_BROADCASTING;

// 	// Shut down the camera
// 	options.publishCameraTrack = false;
// 	// Set both clients as the BROADCASTER.
// 	options.clientRoleType = CLIENT_ROLE_BROADCASTER;

// 	string token_str = token.utf8().ptr();
// 	string channel_str = channel_id.utf8().ptr();
// 	string username_str = username.utf8().ptr();
// 	// int ret = rtc_engine->joinChannel(token_str.c_str(), channel_str.c_str(), user_id, options);
// 	int ret = rtc_engine->joinChannelWithUserAccount(token_str.c_str(), channel_str.c_str(), username_str.c_str());
// 	printf("join channel return %i\n", ret);
// }

void AgoraClient::join_channel(Ref<JoinChannelParameter> params) {

	ChannelMediaOptions options;
	// For a voice call scenario, set the channel profile as BROADCASTING.
	options.channelProfile = CHANNEL_PROFILE_LIVE_BROADCASTING;

	// Shut down the camera
	options.publishCameraTrack = false;
	// Set both clients as the BROADCASTER.
	options.clientRoleType = CLIENT_ROLE_BROADCASTER;
	
	int ret = rtc_engine->joinChannelWithUserAccount(params->get_token().ascii().get_data(),
		params->get_channel_id().ascii().get_data(),
		get_config()->get_username().ascii().get_data(), options);
		print_line(String("join channel return: ") + String(itos(ret)));
}



void AgoraClient::leave_channel(){

    if (rtc_engine == NULL){
        WARN_PRINT("*RTC engine not initialized*");
        return;
    }
	rtc_engine->leaveChannel();
}

void AgoraClient::set_self_mute(bool value)
{
	if (!rtc_engine) { return; }
	rtc_engine->muteLocalAudioStream(value);
}

void AgoraClient::set_mute_user(const String& user_id, bool value)
{
	agora::rtc::UserInfo ui;
	int res = rtc_engine->getUserInfoByUserAccount(user_id.ascii().get_data(), &ui);
	if (res < 0)
	{
		print_error("Error getting user info");
	}
	else
	{
		rtc_engine->muteRemoteAudioStream(ui.uid, value);
	}
}

void AgoraClient::shutdown() {

	if (!rtc_engine)
		return;

	leave_channel();
	rtc_engine->release();
	rtc_engine = NULL;
}

void AgoraClient::_notification(int what)
{
	switch(what) {
		case NOTIFICATION_PROCESS:
			process();
		break;
	}
	
}

std::vector<uid_t> AgoraClient::get_users()
{
	return std::vector<uid_t>();
}

void AgoraClient::update_position(Ref<AudioClientUpdatePosition> value)
{
	if (!rtc_engine) { return; }

	PoolVector3Array targets = value->get_targets();
	PoolStringArray names = value->get_names();
	for (int index = 0; index < names.size(); index++)
	{
		String name = names[index];
		agora::rtc::UserInfo ui;
		int res = rtc_engine->getUserInfoByUserAccount(name.ascii().get_data(), &ui);
		if (res < 0)
		{
			print_line("Failing to get user info");
			continue;
		}
		else
		{
			agora::SpatialAudioParams ap;
			ap.speaker_distance = value->get_origin().distance_to(targets[index]);
			ap.speaker_azimuth = value->get_origin().signed_angle_to(targets[index], up);
			ap.speaker_elevation = value->get_origin().signed_angle_to(targets[index], right);
			rtc_engine->setRemoteUserSpatialAudioParams(ui.uid, ap);
		}
	}
}

void AgoraClient::add_user(const String& username, uid_t user_id)
{
	std::string s(username.ascii().get_data());
	username_uid.insert({s, user_id});
	uid_username.insert({user_id, s});
}

bool AgoraClient::is_muted() const
{
	// rtc_engine->get
	// rtc_engine->setRemoteUserSpatialAudioParams()
	return true;
}

void AgoraClient::remove_user(uid_t user_id)
{

}

void AgoraClient::process()
{
	// print_line("== ANITYA == Sending data...");
	if (!rtc_engine) { return; }
	PoolByteArray v = get_voice();
	const int total = package_size * package_per_loop;
	if (v.size() < total)
	{
		return;
	}

	for (int index = 0; index < package_per_loop; index++)
	{
		
		memcpy(send_data.write().ptr(), v.write().ptr() + (index * package_size), package_size);
		// print_line("== ANITYA == Sending data...");
		rtc_engine->sendStreamMessage(user_id, (const char*)send_data.read().ptr(), package_size);

	}

	v.resize(v.size() - total);
}

void AgoraClient::on_client_data_received(uid_t uid, const char* data, size_t length)
{
	PoolVector2Array d;
	d.resize(length / 4);

	for (int index = 0; index < length / 4; index++)
	{
		Vector2* v = d.write().ptr() + index;
		v->x = v->y = *((float*)data[index * 4]);
	}

	emit_signal("data_received", user_id, d);
}

void AgoraClient::_bind_methods(){
    ClassDB::bind_method(D_METHOD("init", "config"), &AgoraClient::init);
	// ClassDB::bind_method(D_METHOD("_process", "delta"), &AgoraClient::_process);
    // ClassDB::bind_method(D_METHOD("set_app_id", "app_id"), &AgoraClient::set_app_id);
    ClassDB::bind_method(D_METHOD("join_channel", "config"), &AgoraClient::join_channel);
    ClassDB::bind_method(D_METHOD("leave_channel"), &AgoraClient::leave_channel);
	ClassDB::bind_method(D_METHOD("send_voice"), &AgoraClient::send_voice);
	ClassDB::bind_method(D_METHOD("set_self_mute", "value"), &AgoraClient::set_self_mute);
	ClassDB::bind_method(D_METHOD("set_mute_user", "user_id"), &AgoraClient::set_mute_user);
	ClassDB::bind_method(D_METHOD("update_positions", "positions"), &AgoraClient::update_position);
	ADD_SIGNAL(MethodInfo("data_received", PropertyInfo(Variant::INT, "user_id"), PropertyInfo(Variant::POOL_VECTOR2_ARRAY, "data")));
}

AgoraClient::AgoraClient() {
    initialized = false;
	rtc_engine = NULL;
	event_handler = NULL;
	send_data.resize(package_size);
};

AgoraClient::~AgoraClient() {

	shutdown();
};




void RTCEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed){
    WARN_PRINT("User:" + itos(uid) + " - joined to channel:" + String(channel));
	// client->set_user_id(uid);
    joined_to_channel = true;
}
void RTCEventHandler::onLeaveChannel(const RtcStats& stat){
    joined_to_channel  = false;
}

void RTCEventHandler::onError(int err, const char *msg){
    WARN_PRINT("On Error: Error code:" + itos(err));
}


void RTCEventHandler::onConnectionStateChanged(CONNECTION_STATE_TYPE state, CONNECTION_CHANGED_REASON_TYPE reason){
    current_state = state;
	printf("connection state changed to %i\n", (int)state);
}

void RTCEventHandler::onLocalUserRegistered(uid_t uid, const char* userAccount){
	printf("User with id '%d' joined", (int)uid);
    // current_state = state;
	// printf("connection state changed to %i\n", (int)state);
}

void RTCEventHandler::onUserInfoUpdated(uid_t uid, const agora::rtc::UserInfo& info)
{
	print_line(String("User updates id: ") + String(itos(uid) + String(" userAccount: ") + String(info.userAccount)));
	// print_line(String("On user added: ") + String(itos(uid)));
	// client->add_user(String(info.userAccount), uid);
}

void RTCEventHandler::onStreamMessage(uid_t userId, int streamId, const char* data, size_t length, uint64_t sentTs)
{
	// client->on_client_data_received(userId, data, length);
}


