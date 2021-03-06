#include "jid.h"

using namespace Metre;

void Jid::parse(std::string const & s) {
	ssize_t at_pos{-1};
	ssize_t slash_pos{-1};
	for (size_t c{0}; c != s.length(); ++c) {
		switch(s[c]) {
		case '@':
		  if (at_pos < 0) {
				at_pos = c;
			}
			break;
		case '/':
			slash_pos = c;
			goto loop_exit;
		}
	}
loop_exit:
	if (at_pos >= 0) {
		m_local.emplace(s.data(), at_pos);
	}
	if (slash_pos >= 0) {
		m_resource.emplace(s.data() + slash_pos + 1, s.length() - slash_pos - 1);
	}
	if (at_pos < 0) {
		at_pos = 0;
	} else {
		++at_pos;
	}
	if (at_pos == 0 && slash_pos < 0) {
		m_domain = s;
	} else {
		if (slash_pos < 0) {
			slash_pos = s.length();
		}
		slash_pos -= at_pos;
		m_domain.assign(s.data() + at_pos, slash_pos);
	}
}

std::string const & Jid::full() const {
	if (!m_resource) {
		return bare();
	}
	if (!m_full) {
		m_full.emplace();
		if (m_local) {
			*m_full += *m_local;
			*m_full += "@";
		}
		*m_full += m_domain;
		if (m_resource) {
			*m_full += "/";
			*m_full += *m_resource;
		}
	}
	return *m_full;
}

std::string const & Jid::bare() const {
	if (!m_bare) {
		m_bare.emplace();
		if (m_local) {
			*m_bare += *m_local;
			*m_bare += "@";
		}
		m_bare.value() += m_domain;
	}
	return *m_bare;
}
