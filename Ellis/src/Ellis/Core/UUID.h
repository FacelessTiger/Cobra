#pragma once

#include <xhash>

namespace Ellis {

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
	struct hash<Ellis::UUID>
	{
		std::size_t operator()(const Ellis::UUID& uuid) const
		{
			return (uint64_t)uuid;
		}
	};

}