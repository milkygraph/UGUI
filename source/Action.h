#pragma once
#include <vector>
#include <functional>

template <typename ...Args>
class Action {
public:
	Action(const char* name = "") : m_Name(name) {}
	~Action() = default;
	void Subscribe(std::function<void(Args...)> func) {
		m_Subscribers.push_back(func);
	}

	void Trigger(Args... args) {
		for (auto& subscriber : m_Subscribers) {
			subscriber(args...);
		}
	}

private:
	const char* m_Name{};
	std::vector<std::function<void(Args...)>> m_Subscribers;
};
