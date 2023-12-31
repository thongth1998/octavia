.. -*- rst -*-

List Listeners
==============

.. rest_method:: GET /v2/lbaas/listeners

Lists all listeners for the project.

Use the ``fields`` query parameter to control which fields are
returned in the response body. Additionally, you can filter results
by using query string parameters. For information, see :ref:`filtering`.

Administrative users can specify a project ID that is different than their own
to list listeners for other projects.

The list might be empty.

.. rest_status_code:: success ../http-status.yaml

   - 200

.. rest_status_code:: error ../http-status.yaml

   - 400
   - 401
   - 500

Request
-------

.. rest_parameters:: ../parameters.yaml

   - fields: fields
   - project_id: project_id_query

Curl Example
------------

.. literalinclude:: examples/listeners-list-curl
   :language: bash

Response Parameters
-------------------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up
   - allowed_cidrs: allowed_cidrs
   - alpn_protocols: alpn_protocols-listener
   - client_authentication: client_authentication
   - client_ca_tls_container_ref: client_ca_tls_container_ref
   - client_crl_container_ref: client_crl_container_ref
   - connection_limit: connection_limit
   - created_at: created_at
   - default_pool_id: default_pool_id
   - default_tls_container_ref: default_tls_container_ref
   - description: description
   - hsts_include_subdomains: hsts_include_subdomains
   - hsts_max_age: hsts_max_age
   - hsts_preload: hsts_preload
   - id: listener-id
   - insert_headers: insert_headers
   - l7policies: l7policy-ids
   - listener: listener
   - loadbalancers: loadbalancer-ids
   - name: name
   - operating_status: operating_status
   - project_id: project_id
   - protocol: protocol
   - protocol_port: protocol_port
   - provisioning_status: provisioning_status
   - sni_container_refs: sni_container_refs
   - tags: tags
   - timeout_client_data: timeout_client_data
   - timeout_member_connect: timeout_member_connect
   - timeout_member_data: timeout_member_data
   - timeout_tcp_inspect: timeout_tcp_inspect
   - tls_ciphers: tls_ciphers
   - tls_versions: tls_versions
   - updated_at: updated_at

Response Example
----------------

.. literalinclude:: examples/listeners-list-response.json
   :language: javascript

Create Listener
===============

.. rest_method:: POST /v2/lbaas/listeners

Creates a listener for a load balancer.

The listener configures a port and protocol for the load balancer to listen
on for incoming requests.  A load balancer may have zero or more listeners
configured.

This operation provisions a new listener by using the configuration that
you define in the request object. After the API validates the request and
starts the provisioning process, the API returns a response object that
contains a unique ID and the status of provisioning the listener.

In the response, the listener :ref:`provisioning status<prov_status>` is
``ACTIVE``, ``PENDING_CREATE``, or ``ERROR``.

If the status is ``PENDING_CREATE``, issue GET
``/v2/lbaas/listeners/{listener_id}`` to view the progress of
the provisioning operation. When the listener status changes
to ``ACTIVE``, the listener is successfully provisioned and
is ready for further configuration.

If the API cannot fulfill the request due to insufficient data or
data that is not valid, the service returns the HTTP ``Bad Request
(400)`` response code with information about the failure in the
response body. Validation errors require that you correct the error
and submit the request again.

Specifying a project_id is deprecated.  The listener will inherit
the project_id of the parent load balancer.

You can configure all documented features of the listener at creation time by
specifying the additional elements or attributes in the request.

To create a listener, the parent load balancer must have an ``ACTIVE``
provisioning status.

.. rest_status_code:: success ../http-status.yaml

   - 201

.. rest_status_code:: error ../http-status.yaml

   - 400
   - 401
   - 403
   - 404
   - 409
   - 500
   - 503

Request
-------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up-default-optional
   - allowed_cidrs: allowed_cidrs-optional
   - alpn_protocols: alpn_protocols-listener-optional
   - client_authentication: client_authentication-optional
   - client_ca_tls_container_ref: client_ca_tls_container_ref-optional
   - client_crl_container_ref: client_crl_container_ref-optional
   - connection_limit: connection_limit-optional
   - default_pool: pool-optional
   - default_pool_id: default_pool_id-optional
   - default_tls_container_ref: default_tls_container_ref-optional
   - description: description-optional
   - hsts_include_subdomains: hsts_include_subdomains-optional
   - hsts_max_age: hsts_max_age-optional
   - hsts_preload: hsts_preload-optional
   - insert_headers: insert_headers-optional
   - l7policies: l7policies-optional
   - listeners: listener
   - loadbalancer_id: loadbalancer-id
   - name: name-optional
   - project_id: project_id-optional-deprecated
   - protocol: protocol
   - protocol_port: protocol_port
   - sni_container_refs: sni_container_refs-optional
   - tags: tags-optional
   - timeout_client_data: timeout_client_data-optional
   - timeout_member_connect: timeout_member_connect-optional
   - timeout_member_data: timeout_member_data-optional
   - timeout_tcp_inspect: timeout_tcp_inspect-optional
   - tls_ciphers: tls_ciphers-optional
   - tls_versions: tls_versions-optional

.. _header_insertions:

Supported HTTP Header Insertions
--------------------------------

.. note::
   Both the key and the values are always specified as strings when specifying
   header insertions.


+-------------------------+--------+------------------------------------------------+
| Key                     | Value  | Description                                    |
+=========================+========+================================================+
| X-Forwarded-For         | string | When "``true``" a ``X-Forwarded-For`` header   |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member`` that specifies the client IP        |
|                         |        | address.                                       |
+-------------------------+--------+------------------------------------------------+
| X-Forwarded-Port        | string | When "``true``" a ``X-Forwarded-Port`` header  |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member`` that specifies the listener port.   |
+-------------------------+--------+------------------------------------------------+
| X-Forwarded-Proto       | string | When "``true``" a ``X-Forwarded-Proto`` header |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member``. HTTP for the HTTP listener         |
|                         |        | protocol type, HTTPS for the TERMINATED_HTTPS  |
|                         |        | listener protocol type.                        |
|                         |        | **New in version 2.1**                         |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-Verify     | string | When "``true``" a ``X-SSL-Client-Verify``      |
|                         |        | header is inserted into the request to the     |
|                         |        | backend ``member`` that contains 0 if the      |
|                         |        | client authentication was successful, or an    |
|                         |        | result error number greater than 0 that align  |
|                         |        | to the openssl veryify error codes.            |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-Has-Cert   | string | When "``true``" a ``X-SSL-Client-Has-Cert``    |
|                         |        | header is inserted into the request to the     |
|                         |        | backend ``member`` that is ''true'' if a client|
|                         |        | authentication certificate was presented, and  |
|                         |        | ''false'' if not. Does not indicate validity.  |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-DN         | string | When "``true``" a ``X-SSL-Client-DN`` header   |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member`` that contains the full              |
|                         |        | Distinguished Name of the certificate          |
|                         |        | presented by the client.                       |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-CN         | string | When "``true``" a ``X-SSL-Client-CN`` header   |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member`` that contains the Common Name from  |
|                         |        | the full Distinguished Name of the certificate |
|                         |        | presented by the client.                       |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Issuer            | string | When "``true``" a ``X-SSL-Issuer`` header is   |
|                         |        | inserted into the request to the backend       |
|                         |        | ``member`` that contains the full              |
|                         |        | Distinguished Name of the client certificate   |
|                         |        | issuer.                                        |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-SHA1       | string | When "``true``" a ``X-SSL-Client-SHA1`` header |
|                         |        | is inserted into the request to the backend    |
|                         |        | ``member`` that contains the SHA-1 fingerprint |
|                         |        | of the certificate presented by the client in  |
|                         |        | hex string format.                             |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-Not-Before | string | When "``true``" a ``X-SSL-Client-Not-Before``  |
|                         |        | header is inserted into the request to the     |
|                         |        | backend ``member`` that contains the start     |
|                         |        | date presented by the client as a formatted    |
|                         |        | string YYMMDDhhmmss[Z].                        |
+-------------------------+--------+------------------------------------------------+
| X-SSL-Client-Not-After  | string | When "``true``" a ``X-SSL-Client-Not-After``   |
|                         |        | header is inserted into the request to the     |
|                         |        | backend ``member`` that contains the end date  |
|                         |        | presented by the client as a formatted string  |
|                         |        | YYMMDDhhmmss[Z].                               |
+-------------------------+--------+------------------------------------------------+

Request Example
----------------

.. literalinclude:: examples/listener-create-request.json
   :language: javascript

Curl Example
------------

.. literalinclude:: examples/listener-create-curl
   :language: bash

Response Parameters
-------------------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up
   - allowed_cidrs: allowed_cidrs
   - alpn_protocols: alpn_protocols-listener
   - client_authentication: client_authentication
   - client_ca_tls_container_ref: client_ca_tls_container_ref
   - client_crl_container_ref: client_crl_container_ref
   - connection_limit: connection_limit
   - created_at: created_at
   - default_pool_id: default_pool_id
   - default_tls_container_ref: default_tls_container_ref
   - description: description
   - hsts_include_subdomains: hsts_include_subdomains
   - hsts_max_age: hsts_max_age
   - hsts_preload: hsts_preload
   - id: listener-id
   - insert_headers: insert_headers
   - l7policies: l7policy-ids
   - listener: listener
   - loadbalancers: loadbalancer-ids
   - name: name
   - operating_status: operating_status
   - project_id: project_id
   - protocol: protocol
   - protocol_port: protocol_port
   - provisioning_status: provisioning_status
   - sni_container_refs: sni_container_refs
   - tags: tags
   - timeout_client_data: timeout_client_data
   - timeout_member_connect: timeout_member_connect
   - timeout_member_data: timeout_member_data
   - timeout_tcp_inspect: timeout_tcp_inspect
   - tls_ciphers: tls_ciphers
   - tls_versions: tls_versions
   - updated_at: updated_at

Response Example
----------------

.. literalinclude:: examples/listener-create-response.json
   :language: javascript

Show Listener details
=====================

.. rest_method:: GET /v2/lbaas/listeners/{listener_id}

Shows the details of a listener.

If you are not an administrative user and the parent load balancer does not
belong to your project, the service returns the HTTP ``Forbidden (403)``
response code.

This operation does not require a request body.

.. rest_status_code:: success ../http-status.yaml

   - 200

.. rest_status_code:: error ../http-status.yaml

   - 401
   - 403
   - 404
   - 500

Request
-------

.. rest_parameters:: ../parameters.yaml

   - fields: fields
   - listener_id: path-listener-id

Curl Example
------------

.. literalinclude:: examples/listener-show-curl
   :language: bash

Response Parameters
-------------------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up
   - allowed_cidrs: allowed_cidrs
   - alpn_protocols: alpn_protocols-listener
   - client_authentication: client_authentication
   - client_ca_tls_container_ref: client_ca_tls_container_ref
   - client_crl_container_ref: client_crl_container_ref
   - connection_limit: connection_limit
   - created_at: created_at
   - default_pool_id: default_pool_id
   - default_tls_container_ref: default_tls_container_ref
   - description: description
   - hsts_include_subdomains: hsts_include_subdomains
   - hsts_max_age: hsts_max_age
   - hsts_preload: hsts_preload
   - id: listener-id
   - insert_headers: insert_headers
   - l7policies: l7policy-ids
   - listener: listener
   - loadbalancers: loadbalancer-ids
   - name: name
   - operating_status: operating_status
   - project_id: project_id
   - protocol: protocol
   - protocol_port: protocol_port
   - provisioning_status: provisioning_status
   - sni_container_refs: sni_container_refs
   - tags: tags
   - timeout_client_data: timeout_client_data
   - timeout_member_connect: timeout_member_connect
   - timeout_member_data: timeout_member_data
   - timeout_tcp_inspect: timeout_tcp_inspect
   - tls_ciphers: tls_ciphers
   - tls_versions: tls_versions
   - updated_at: updated_at

Response Example
----------------

.. literalinclude:: examples/listener-show-response.json
   :language: javascript

Update a Listener
=================

.. rest_method:: PUT /v2/lbaas/listeners/{listener_id}

Update an existing listener.

If the request is valid, the service returns the ``Accepted (202)``
response code. To confirm the update, check that the listener provisioning
status is ``ACTIVE``. If the status is ``PENDING_UPDATE``, use a GET
operation to poll the listener object for changes.

This operation returns the updated listener object with the
``ACTIVE``, ``PENDING_UPDATE``, or ``ERROR`` provisioning status.

.. rest_status_code:: success ../http-status.yaml

   - 202

.. rest_status_code:: error ../http-status.yaml

   - 400
   - 401
   - 403
   - 404
   - 409
   - 500

Request
-------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up-default-optional
   - allowed_cidrs: allowed_cidrs-optional
   - alpn_protocols: alpn_protocols-listener-optional
   - client_authentication: client_authentication-optional
   - client_ca_tls_container_ref: client_ca_tls_container_ref-optional
   - client_crl_container_ref: client_crl_container_ref-optional
   - connection_limit: connection_limit-optional
   - default_pool_id: default_pool_id-optional
   - default_tls_container_ref: default_tls_container_ref-optional
   - description: description-optional
   - hsts_include_subdomains: hsts_include_subdomains-optional
   - hsts_max_age: hsts_max_age-optional
   - hsts_preload: hsts_preload-optional
   - insert_headers: insert_headers-optional
   - listener_id: path-listener-id
   - name: name-optional
   - sni_container_refs: sni_container_refs-optional
   - tags: tags-optional
   - timeout_client_data: timeout_client_data-optional
   - timeout_member_connect: timeout_member_connect-optional
   - timeout_member_data: timeout_member_data-optional
   - timeout_tcp_inspect: timeout_tcp_inspect-optional
   - tls_ciphers: tls_ciphers-optional
   - tls_versions: tls_versions-optional

Request Example
---------------

.. literalinclude:: examples/listener-update-request.json
   :language: javascript

Curl Example
------------

.. literalinclude:: examples/listener-update-curl
   :language: bash

Response Parameters
-------------------

.. rest_parameters:: ../parameters.yaml

   - admin_state_up: admin_state_up
   - allowed_cidrs: allowed_cidrs
   - alpn_protocols: alpn_protocols-listener
   - client_authentication: client_authentication
   - client_ca_tls_container_ref: client_ca_tls_container_ref
   - client_crl_container_ref: client_crl_container_ref
   - connection_limit: connection_limit
   - created_at: created_at
   - default_pool_id: default_pool_id
   - default_tls_container_ref: default_tls_container_ref
   - description: description
   - hsts_include_subdomains: hsts_include_subdomains
   - hsts_max_age: hsts_max_age
   - hsts_preload: hsts_preload
   - id: listener-id
   - insert_headers: insert_headers
   - l7policies: l7policy-ids
   - listener: listener
   - loadbalancers: loadbalancer-ids
   - name: name
   - operating_status: operating_status
   - project_id: project_id
   - protocol: protocol
   - protocol_port: protocol_port
   - provisioning_status: provisioning_status
   - sni_container_refs: sni_container_refs
   - tags: tags
   - timeout_client_data: timeout_client_data
   - timeout_member_connect: timeout_member_connect
   - timeout_member_data: timeout_member_data
   - timeout_tcp_inspect: timeout_tcp_inspect
   - tls_ciphers: tls_ciphers
   - tls_versions: tls_versions
   - updated_at: updated_at

Response Example
----------------

.. literalinclude:: examples/listener-update-response.json
   :language: javascript

Remove a Listener
=================

.. rest_method:: DELETE /v2/lbaas/listeners/{listener_id}

Removes a listener and its associated configuration from the project.

The API immediately purges any and all configuration data, depending on the
configuration settings. You cannot recover it.

.. rest_status_code:: success ../http-status.yaml

   - 204

.. rest_status_code:: error ../http-status.yaml

   - 400
   - 401
   - 403
   - 404
   - 409
   - 500

Request
-------

.. rest_parameters:: ../parameters.yaml

   - listener_id: path-listener-id

Curl Example
------------

.. literalinclude:: examples/listener-delete-curl
   :language: bash

Response
--------

There is no body content for the response of a successful DELETE request.

Get Listener statistics
=======================

.. rest_method:: GET /v2/lbaas/listeners/{listener_id}/stats

Shows the current statistics for a listener.

This operation returns the statistics of a listener object identified
by listener_id.

If you are not an administrative user and the parent load balancer does not
belong to your project, the service returns the HTTP ``Forbidden (403)``
response code.

This operation does not require a request body.

.. rest_status_code:: success ../http-status.yaml

   - 200

.. rest_status_code:: error ../http-status.yaml

   - 401
   - 403
   - 404
   - 500

Request
-------

.. rest_parameters:: ../parameters.yaml

   - listener_id: path-listener-id

Curl Example
------------

.. literalinclude:: examples/listener-stats-curl
   :language: bash

Response Parameters
-------------------

.. rest_parameters:: ../parameters.yaml

   - stats: stats
   - active_connections: active_connections
   - bytes_in: bytes_in
   - bytes_out: bytes_out
   - request_errors: request_errors
   - total_connections: total_connections

Response Example
----------------

.. literalinclude:: examples/listener-stats-response.json
   :language: javascript
