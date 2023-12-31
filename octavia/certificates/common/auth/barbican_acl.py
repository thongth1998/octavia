# Copyright (c) 2014 Rackspace US, Inc
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.

"""
Barbican ACL auth class for Barbican certificate handling
"""
from barbicanclient import client as barbican_client
from keystoneauth1 import session
from keystoneauth1 import token_endpoint

from oslo_config import cfg
from oslo_log import log as logging
from oslo_utils import excutils

from octavia.certificates.common import barbican as barbican_common
from octavia.common import keystone

LOG = logging.getLogger(__name__)
CONF = cfg.CONF


class BarbicanACLAuth(barbican_common.BarbicanAuth):
    _barbican_client = None

    @classmethod
    def get_barbican_client(cls, project_id=None):
        if not cls._barbican_client:
            try:
                ksession = keystone.KeystoneSession()
                cls._barbican_client = barbican_client.Client(
                    session=ksession.get_session(),
                    region_name=CONF.certificates.region_name,
                    interface=CONF.certificates.endpoint_type
                )
            except Exception:
                with excutils.save_and_reraise_exception():
                    LOG.exception("Error creating Barbican client")
        return cls._barbican_client

    @classmethod
    def ensure_secret_access(cls, context, ref):
        # get a normal session
        ksession = keystone.KeystoneSession()
        user_id = ksession.get_service_user_id()

        # use barbican client to set the ACLs
        bc = cls.get_barbican_client_user_auth(context)
        acl = bc.acls.get(ref)
        read_oper = acl.get('read')
        if user_id not in read_oper.users:
            read_oper.users.append(user_id)
            acl.submit()

    @classmethod
    def revoke_secret_access(cls, context, ref):
        # get a normal session
        ksession = keystone.KeystoneSession()
        user_id = ksession.get_service_user_id()

        # use barbican client to set the ACLs
        bc = cls.get_barbican_client_user_auth(context)
        acl = bc.acls.get(ref)
        read_oper = acl.get('read')
        if user_id in read_oper.users:
            read_oper.users.remove(user_id)
            acl.submit()

    @classmethod
    def get_barbican_client_user_auth(cls, context):
        barbican_endpoint = CONF.certificates.endpoint
        if not barbican_endpoint:
            ksession = keystone.KeystoneSession().get_session()
            endpoint_data = ksession.get_endpoint_data(
                service_type='key-manager',
                region_name=CONF.certificates.region_name,
                interface=CONF.certificates.endpoint_type)
            barbican_endpoint = endpoint_data.catalog_url

        auth_token = token_endpoint.Token(barbican_endpoint,
                                          context.auth_token)

        user_session = session.Session(
            auth=auth_token,
            verify=CONF.certificates.ca_certificates_file)
        return barbican_client.Client(
            session=user_session,
            endpoint=barbican_endpoint)
