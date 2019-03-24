#ifndef NL_SESSION_H
#define NL_SESSION_H

#include <string>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/stream.h>

class PlayedStats {
public:
	PlayedStats() : LevelID(0), Moves(0), Falls(0), TimeToComplete(0), Completed(false) { }
	PlayedStats(std::string levelCategory, uint32 levelId) :	LevelCategory(levelCategory), 
																LevelID(levelId), 
																Moves(0),
																Falls(0),
																TimeToComplete(0),
																Completed(false) { };
	std::string LevelCategory;
	uint32 LevelID;
	uint32 Moves;
	uint32 Falls;
	double TimeToComplete;
	bool Completed;

	void serial(NLMISC::IStream &s) {
		s.serial(LevelCategory);
		s.serial(Moves);
		s.serial(Falls);
		s.serial(TimeToComplete);
		s.serial(Completed);
	}
};

class Session {
public:
	Session() { };
	Session(std::string sessionName) : m_sessionName(sessionName) { };

	void serial(NLMISC::IStream &s) {
		s.serial(m_sessionName);
		s.serialContPtr(m_sessionHistory);
	}
	
	typedef std::list<PlayedStats *> TPlayedStatsList;
	TPlayedStatsList &getSessionHistory() { return m_sessionHistory; }

	PlayedStats &getCurrentPlay() { return *m_currentPlay; }

	void incrementMoves();
	void incrementFalls();
	void finalizePlay();
	void startNewPlay(std::string levelCategory, uint32 levelId);

	void setSessionName(std::string sessionName) { m_sessionName = sessionName; }
	std::string getSessionName() { return m_sessionName; }
private:
	TPlayedStatsList m_sessionHistory;
	PlayedStats		*m_currentPlay;
	std::string		 m_sessionName;

};

class SessionManager {
	SessionManager() { };
	~SessionManager() { };

	NLMISC_SAFE_SINGLETON_DECL(SessionManager);
public:
	/// Initialize, load a list of available sessions
	void init();
	void save();
	Session &getCurrentSession() { return *m_currentSession; }
	Session &getNewSession(std::string sessionName);

	typedef std::list<Session *> TSessionList;
	TSessionList	&getSessions() { return m_sessionList; }
private:
	Session			*m_currentSession;
	TSessionList	 m_sessionList;
};

#endif // NL_SESSION_H