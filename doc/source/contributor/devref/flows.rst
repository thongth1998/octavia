========================
Octavia Controller Flows
========================

Octavia uses OpenStack TaskFlow to orchestrate the actions the Octavia
controller needs to take while managing load balancers.

This document is meant as a reference for the key flows used in the
Octavia controller.

The following are flow diagrams for the **amphora V2** driver.

.. toctree::
    :maxdepth: 1

    flow_diagrams_v2/AmphoraFlows.rst
    flow_diagrams_v2/HealthMonitorFlows.rst
    flow_diagrams_v2/L7PolicyFlows.rst
    flow_diagrams_v2/L7RuleFlows.rst
    flow_diagrams_v2/ListenerFlows.rst
    flow_diagrams_v2/LoadBalancerFlows.rst
    flow_diagrams_v2/MemberFlows.rst
    flow_diagrams_v2/PoolFlows.rst
