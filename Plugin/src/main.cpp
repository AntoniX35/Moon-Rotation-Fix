bool nextgen;

void MessageHandler(F4SE::MessagingInterface::Message* a_message)
{
	switch (a_message->type) {
	case F4SE::MessagingInterface::kPostLoad:
		break;
	case F4SE::MessagingInterface::kGameDataReady:
		{	
			constexpr std::uint8_t bytes[] = { 0x03 };
			auto& trampoline = F4SE::GetTrampoline();
			if (!nextgen) {
				REL::Relocation<std::uintptr_t> FixAddress{ REL::ID(114988), 0x1E2};
												
				REL::safe_write(FixAddress.address(), &bytes, sizeof(bytes));
			}
			else {
				REL::Relocation<std::uintptr_t> FixAddress{ REL::ID(2208804), 0x1F7};
												
				REL::safe_write(FixAddress.address(), &bytes, sizeof(bytes));
			}
		}
		break;
	default:
		break;
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_F4SE, F4SE::PluginInfo* a_info)
{
	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = "MoonRotationFix";
	a_info->version = 1.1;

	return true;
}

DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_F4SE)
{
#ifndef NDEBUG
	MessageBoxA(NULL, "Loaded. You can now attach the debugger or continue execution.", Plugin::NAME.data(), NULL);
#endif
	auto ver = a_F4SE->RuntimeVersion();
	
	F4SE::Init(a_F4SE);
	if (ver >= F4SE::RUNTIME_1_10_980) {
		nextgen = true;
	}
	DKUtil::Logger::Init(Plugin::NAME, std::to_string(Plugin::Version));
	INFO("{} v{} loaded."sv, Plugin::NAME, Plugin::Version);
	
	const auto messaging = F4SE::GetMessagingInterface();
	messaging->RegisterListener(MessageHandler);

	return true;
}
