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
	std::string app_id(p_config->get_app_id().utf8().ptr());
	engine_context.appId = app_id.c_str();
	WARN_PRINT("App ID: " + String(engine_context.appId));
    int err = rtc_engine->initialize(engine_context);
    err = rtc_engine->setChannelProfile(CHANNEL_PROFILE_LIVE_BROADCASTING);
	WARN_PRINT("Agora version:" + String(rtc_engine->getVersion(0)));
	packet_observer.client = this;
	std::string user(p_config->get_username().utf8().ptr());
	WARN_PRINT("userAccount: " + String(user.c_str()));
	int value = rtc_engine->registerLocalUserAccount(engine_context.appId, user.c_str());
	rtc_engine->enableSpatialAudio(true);
	rtc_engine->enableSoundPositionIndication(true);
	WARN_PRINT("Register user:" + String(itos(value)));

	// rtc_engine->sendStreamMessage

    initialized = true;
};


void AgoraClient::join_channel(Ref<JoinChannelParameter> params)
{
	ChannelMediaOptions options;
	// For a voice call scenario, set the channel profile as BROADCASTING.
	options.channelProfile = CHANNEL_PROFILE_LIVE_BROADCASTING;
	// Shut down the camera
	options.publishCameraTrack = false;
	// Set both clients as the BROADCASTER.
	options.clientRoleType = CLIENT_ROLE_BROADCASTER;
	
	std::string token(params->get_token().utf8().ptr());
	std::string channel(params->get_channel_id().utf8().ptr());
	std::string user(get_config()->get_username().utf8().ptr());

	int ret = rtc_engine->joinChannelWithUserAccount(token.c_str(), channel.c_str(), user.c_str(), options);
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

	PoolVector3Array targets = value->_get_targets();
	PoolStringArray names = value->_get_ids();
	for (int index = 0; index < names.size(); index++)
	{
		String name = names[index];
		agora::rtc::UserInfo ui;
		std::string n(name.utf8().ptr());
		print_line(String("Updating positions"));
		int res = rtc_engine->getUserInfoByUserAccount(n.c_str(), &ui);
		if (res < 0)
		{
			print_line("Failing to get user info");
			continue;
		}
		else
		{
			const Vector3& o = value->_get_origin();
			Vector3 distance = targets[index] - o;
			Vector3 diff = distance.normalized();
			double pan = diff.dot(forward);
			double gain = distance.length();
			rtc_engine->setRemoteVoicePosition(ui.uid, pan, gain);
			agora::SpatialAudioParams ap;
			ap.speaker_distance = distance.length();
			double azimuth = Vector2(o.x, o.z).angle();
			double elevation = Vector2(o.x, o.y).angle();
			ap.speaker_azimuth = azimuth;
			ap.speaker_elevation = elevation;
			rtc_engine->setRemoteUserSpatialAudioParams(ui.uid, ap);
		}
	}
}

void AgoraClient::add_user(const String& username, uid_t user_id)
{
	std::string s(username.utf8().ptr());
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
	// if (!rtc_engine) { return; }
	// PoolByteArray v = get_voice();
	// const int total = package_size * package_per_loop;
	// if (v.size() < total)
	// {
	// 	return;
	// }

	// for (int index = 0; index < package_per_loop; index++)
	// {
		
	// 	memcpy(send_data.write().ptr(), v.write().ptr() + (index * package_size), package_size);
	// 	// print_line("== ANITYA == Sending data...");
	// 	rtc_engine->sendStreamMessage(user_id, (const char*)send_data.read().ptr(), package_size);

	// }

	// v.resize(v.size() - total);
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

	ClassDB::bind_method(D_METHOD("set_client_id", "client_id"), &AgoraClient::set_client_id);
    ClassDB::bind_method(D_METHOD("get_client_id"), &AgoraClient::get_client_id);
    // ClassDB::bind_method(D_METHOD("_no_set_int"), &AgoraClient::_no_set_int);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "client_id"), "set_client_id", "get_client_id");

    ClassDB::bind_method(D_METHOD("init", "config"), &AgoraClient::init);
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
	client->set_client_id(uid);
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


