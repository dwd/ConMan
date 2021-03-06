#ifndef ELOQUENCE_EXCEPT_HH
#define ELOQUENCE_EXCEPT_HH

#include <exception>

namespace Metre {
	namespace base {
		class xmpp_exception : public std::runtime_error {
		private:
			const char * m_elname; // Always used with string constant
		public:
			xmpp_exception(std::string const & w, const char * elname) : std::runtime_error(w), m_elname(elname) {}
			xmpp_exception(const char * w, const char * elname) : std::runtime_error(w), m_elname(elname) {}

			const char * element_name() const {
				return m_elname;
			}
		};
	}

	// Following macro generates exception classes.
#   define METRE_XMPP_EXCEPT(clsname, def_text, elname) \
	class clsname : public base::xmpp_exception {  \
	public:  \
		clsname() : base::xmpp_exception(def_text, elname) {}  \
		clsname(std::string const & w) : base::xmpp_exception(w, elname) {}  \
		clsname(const char * w) : base::xmpp_exception(w, elname) {}  \
	}

	METRE_XMPP_EXCEPT(bad_format, "Sorry, I cannot process that XML", "bad-format");
	METRE_XMPP_EXCEPT(bad_namespace_prefix, "Required prefix missing", "bad-namespace-prefix");
	METRE_XMPP_EXCEPT(host_unknown, "FQDN not serviced by this entity", "host-unknown");
	METRE_XMPP_EXCEPT(not_authorized, "Not authorized to perform that action", "not-authorized");
	METRE_XMPP_EXCEPT(unsupported_stanza_type, "Couldn't understand that element", "unsupported-stanza-type");
	METRE_XMPP_EXCEPT(undefined_condition, "Very sorry - unhandled internal error", "undefined-condition");

	namespace base {
		class stanza_exception : public std::runtime_error {
		private:
			const char * m_elname;
			const char * m_error_type;
		public:
			stanza_exception(std::string const & w, const char * elname, const char * error_type) : std::runtime_error(w), m_elname(elname), m_error_type(error_type) {}
			stanza_exception(const char * w, const char * elname, const char * error_type) : std::runtime_error(w), m_elname(elname), m_error_type(error_type) {}

			const char * element_name() const {
				return m_elname;
			}

			const char * error_type() const {
				return m_error_type;
			}
		};
	}

	// Another Macro for Stanza errors.
#		define METRE_STANZA_EXCEPT(errname, def_text, def_type, elname) \
	class stanza_##errname : public base::stanza_exception { \
	public: \
		stanza_##errname() : base::stanza_exception(def_text, elname, def_type) {} \
		stanza_##errname(std::string const & w) : base::stanza_exception(w, elname, def_type) {} \
		stanza_##errname(std::string const & w, const char * err_type) : base::stanza_exception(w, elname, err_type) {} \
		stanza_##errname(const char * w) : base::stanza_exception(w, elname, def_type) {} \
		stanza_##errname(const char * w, const char * err_type) : base::stanza_exception(w, elname, err_type) {} \
	}

	METRE_STANZA_EXCEPT(service_unavailable, "This service is not available at this jid", "cancel", "service-unavailable");
	METRE_STANZA_EXCEPT(undefined_condition, "An internal server error occured processing this stanza", "cancel", "undefined-condition");
}

#endif
