/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <utility>
#include <vector>

#include "brave/browser/ipfs/ipfs_host_resolver.h"
#include "chrome/browser/net/secure_dns_config.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "net/base/host_port_pair.h"
#include "net/dns/public/dns_protocol.h"
#include "services/network/network_context.h"

namespace {

net::DnsConfigOverrides GetActualDnsConfig() {
  SecureDnsConfig secure_dns_config =
      SystemNetworkContextManager::GetStubResolverConfigReader()
          ->GetSecureDnsConfiguration(true);

  net::DnsConfigOverrides current_config_overrides;
  current_config_overrides.search = std::vector<std::string>();
  current_config_overrides.attempts = 1;

  current_config_overrides.secure_dns_mode = secure_dns_config.mode();
  if (!secure_dns_config.servers().empty()) {
    current_config_overrides.dns_over_https_servers.emplace(
        secure_dns_config.servers());
  }
  return current_config_overrides;
}

}  // namespace

namespace ipfs {

IPFSHostResolver::IPFSHostResolver(
    network::mojom::NetworkContext* network_context,
    const std::string& prefix)
    : prefix_(prefix) {
  DCHECK(network_context);
  network_context->CreateHostResolver(
      GetActualDnsConfig(), host_resolver_.BindNewPipeAndPassReceiver());
}
IPFSHostResolver::~IPFSHostResolver() {}

void IPFSHostResolver::Resolve(const net::HostPortPair& host,
                               const net::NetworkIsolationKey& isolation_key,
                               net::DnsQueryType dns_query_type,
                               HostTextResultsCallback callback) {
  if (IsRunning())
    Stop();

  network::mojom::ResolveHostParametersPtr parameters =
      network::mojom::ResolveHostParameters::New();
  parameters->dns_query_type = dns_query_type;
  parameters->source = net::HostResolverSource::DNS;
  parameters->cache_usage =
      network::mojom::ResolveHostParameters::CacheUsage::ALLOWED;
  resolving_host_ = host.host();
  resolved_callback_ = std::move(callback);

  GetHostResolver()->ResolveHost(
      net::HostPortPair(prefix_ + resolving_host_, host.port()), isolation_key,
      std::move(parameters), receiver_.BindNewPipeAndPassRemote());
  receiver_.set_disconnect_handler(
      base::BindOnce(&IPFSHostResolver::OnComplete, base::Unretained(this),
                     net::ERR_NAME_NOT_RESOLVED,
                     net::ResolveErrorInfo(net::ERR_FAILED), base::nullopt));
}

bool IPFSHostResolver::IsRunning() const {
  return !resolving_host_.empty();
}

void IPFSHostResolver::Stop() {
  receiver_.reset();
  resolving_host_.erase();
  resolved_callback_.Reset();

  if (stop_called_callback_)
    std::move(stop_called_callback_).Run();
}

void IPFSHostResolver::OnComplete(
    int result,
    const net::ResolveErrorInfo& error_info,
    const base::Optional<net::AddressList>& list) {
  if (result != net::OK)
    VLOG(1) << "DNS resolving error:" << net::ErrorToString(result)
            << " for host: " << prefix_ + resolving_host_;
  Stop();
}

void IPFSHostResolver::OnTextResults(const std::vector<std::string>& results) {
  VLOG(1) << results.size()
          << " TXT records resolved for host: " << prefix_ + resolving_host_;
  if (resolved_callback_)
    std::move(resolved_callback_).Run(resolving_host_, results);
  Stop();
}

}  // namespace ipfs
