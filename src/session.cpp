#include <string>

#include <nel/misc/types_nl.h>
#include <nel/misc/app_context.h>
#include <nel/misc/debug.h>
#include <nel/misc/path.h>
#include <nel/misc/file.h>

#include "session.h"
#include "neloidgame.h"

NLMISC_SAFE_SINGLETON_IMPL(SessionManager)

void SessionManager::init() {
	// Look up all .session files
	std::vector<std::string> files;
	NLMISC::CPath::getFileList("session", files);

	// Load all session files.
	std::vector<std::string>::iterator itr = files.begin();
	while(itr != files.end()) {
		nlinfo("file name: %s", (*itr).c_str());
		NLMISC::CIFile loadSession(NLMISC::CPath::lookup((*itr)));
		Session *newSession = new Session();
		newSession->serial(loadSession);
		m_currentSession = newSession;
		m_sessionList.push_back(newSession);
		itr++;
	}
}

Session &SessionManager::getNewSession(std::string sessionName) {
	Session *session = new Session(sessionName);
	m_currentSession = session;
	m_sessionList.push_back(session);
	return *session;
}

void SessionManager::save() {
	NLMISC::COFile saveSession("./data/"+getCurrentSession().getSessionName()+".session");
	saveSession.serial(getCurrentSession());
	saveSession.close();
}

void Session::incrementMoves() {
	getCurrentPlay().Moves++;
}

void Session::incrementFalls() {
	getCurrentPlay().Falls++;
}

void Session::finalizePlay() {
	getCurrentPlay().Completed=true;
	// TODO make sure we actually compute this.
	getCurrentPlay().TimeToComplete = NeloidGame::getInstance().getTime();
}

void Session::startNewPlay(std::string levelCategory, uint32 levelId) {
	m_currentPlay = new PlayedStats(levelCategory, levelId);
	m_sessionHistory.push_back(m_currentPlay);
}


