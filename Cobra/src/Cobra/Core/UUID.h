#pragma once

namespace Cobra {

	class UUID
	{
	private:
		uint64_t m_UUID;
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }
	};

}

namespace std {

	template<>
	struct hash<Cobra::UUID>
	{
		std::size_t operator()(const Cobra::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}