#pragma once

class Listener : public IGameEventListener2 {
private:
	using GameEvent_t = void(__cdecl*)(IGameEvent*);
	std::map< std::string, std::vector< GameEvent_t > > m_events;
public:
	/* IGameEventListener2 virtual overriding */
	virtual void FireGameEvent(IGameEvent* event) {
		std::string eventName = event->GetName();
		if (m_events.count(eventName) != 0) {
			const auto& events = m_events[eventName];
			for (const auto& e : events) {
				e(event);
			}
		}
	}

	void init();
	void add(const std::string& name, GameEvent_t fn) {
		m_events[name].push_back(fn);
	}

	void register_events() {
		if (m_events.empty())
			return;

		for (const auto& e : m_events) {
			g_csgo.m_game_events->AddListener(this, e.first.data(), false);
		}
	}

	void unregister_events() {
		g_csgo.m_game_events->RemoveListener(this);
	}
};

namespace events {
	void round_start( IGameEvent* evt );
	void round_end( IGameEvent* evt );
	void player_hurt( IGameEvent* evt );
	void bullet_impact( IGameEvent* evt );
	void item_purchase( IGameEvent* evt );
	void player_death( IGameEvent* evt );
	void player_given_c4( IGameEvent* evt );
	void bomb_beginplant( IGameEvent* evt );
	void bomb_abortplant( IGameEvent* evt );
	void bomb_planted( IGameEvent* evt );
    void bomb_beep( IGameEvent* evt );
	void bomb_begindefuse( IGameEvent* evt );
	void bomb_abortdefuse( IGameEvent* evt );
    void bomb_defused( IGameEvent* evt );
    void bomb_exploded( IGameEvent* evt );
}

extern Listener g_listener;