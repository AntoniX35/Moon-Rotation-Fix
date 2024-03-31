namespace MoonRotationFix
{	
	void Install()
	{
		REL::Relocation<std::uintptr_t> FixAddress{ REL::ID(114988), 0x1E2};
		constexpr std::uint8_t bytes[] = { 0x03 };
		REL::safe_write(FixAddress.address(), &bytes, sizeof(bytes));
	}
}

extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Query(const F4SE::QueryInterface* a_f4se, F4SE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= fmt::format(FMT_STRING("{}.log"), Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info("{} v{}", Version::PROJECT, Version::NAME);

	a_info->infoVersion = F4SE::PluginInfo::kVersion;
	a_info->name = Version::PROJECT.data();
	a_info->version = Version::MAJOR;

	if (a_f4se->IsEditor()) {
		logger::critical("loaded in editor");
		return false;
	}

	const auto ver = a_f4se->RuntimeVersion();
	if (ver < F4SE::RUNTIME_1_10_130) {
		logger::critical("unsupported runtime v{}", ver.string());
		return false;
	}

	return true;
}


void MessageCallback(F4SE::MessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case F4SE::MessagingInterface::kGameDataReady:
		{
				MoonRotationFix::Install();
		}
		break;
	case F4SE::MessagingInterface::kNewGame:
		break;
	case F4SE::MessagingInterface::kPostPostLoad:
		break;
	default:
		break;
	}

}


extern "C" DLLEXPORT bool F4SEAPI F4SEPlugin_Load(const F4SE::LoadInterface* a_f4se)
{
	F4SE::Init(a_f4se);

	logger::info("hello world!");

	if (!F4SE::GetMessagingInterface()->RegisterListener(MessageCallback))
	{
		logger::info("Cannot register listener!");
		return false;
	}

	return true;
}
