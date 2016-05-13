#ifndef METRE_CONFIG__HPP
#define METRE_CONFIG__HPP

#include <string>
#include <map>
#include <optional>
#include <memory>
#include <list>

#include "defs.h"
#include "dns.h"

/**
 * OpenSSL forward declarations.
*/

struct ssl_ctx_st; // SSL_CTX
struct x509_store_ctx_st; // X509_STORE_CTX;

namespace Metre {
  class Config {
  public:
    class Domain {
    public:
      std::string const & domain() const {
        return m_domain;
      }
      SESSION_TYPE transport_type() const {
        return m_type;
      }
      bool forward() const {
        return m_forward;
      }
      bool require_tls() const {
        return m_require_tls;
      }
      bool block() const {
        return m_block;
      }
      bool auth_pkix() const {
        return m_auth_pkix;
      }
      bool auth_dialback() const {
        return m_auth_dialback;
      }
      std::optional<std::string> const & auth_secret() const {
        return m_auth_secret;
      }
      struct ssl_ctx_st * ssl_ctx() const {
        return m_ssl_ctx;
      }
      DNS::Address * host_lookup(std::string const & hostname) const;

      /* Loading functions */
      void x509(std::string const & chain, std::string const &);
      void host(std::string const & hostname, uint32_t inaddr);

      Domain(std::string const & domain, SESSION_TYPE transport_type, bool forward, bool require_tls, bool block, bool auth_pkix, bool auth_dialback, std::optional<std::string> && m_auth_secret);
      Domain(Domain const &) = delete;
      Domain(Domain &&) = delete;
      ~Domain();
    private:
      static int verify_callback_cb(int preverify_ok, struct x509_store_ctx_st *);
      std::string m_domain;
      SESSION_TYPE m_type;
      bool m_forward;
      bool m_require_tls;
      bool m_block;
      bool m_auth_pkix;
      bool m_auth_dialback;
      std::optional<std::string> m_auth_secret;
      struct ssl_ctx_st * m_ssl_ctx;
      std::map<std::string, std::unique_ptr<DNS::Address>> m_host_arecs;
      std::map<std::string, std::unique_ptr<DNS::Srv>> m_srvrecs;
      std::map<std::string, std::unique_ptr<DNS::Tlsa>> m_tlsarecs;
    };
    Config(std::string const & filename);

    std::string asString();

    std::string const & default_domain() const {
      return m_default_domain;
    }
    std::string const & runtime_dir() const;
    Domain const & domain(std::string const & domain) const;

    void load(std::string const & filename);

    static Config const & config();
    std::string random_identifier() const;
    std::string const & dialback_secret() const {
      return m_dialback_secret;
    }
    std::string dialback_key(std::string const & id, std::string const & local_domain, std::string const & remote_domain) const;

  private:
    std::string m_config_str;
    std::string m_default_domain;
    std::string m_runtime_dir;
    std::string m_dialback_secret;
    std::map<std::string, std::unique_ptr<Domain>> m_domains;
  };
}

#endif
