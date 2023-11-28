# WebSocket SCADA Server (logger)

Configuration (examples)

- config.json (main)
  - wsblack.json (black-list of clients)
  - wscli.json (list of specific websocket-clients: echo-server, repeater, ...)
  - roles.json (client roles)
  - arh.json (DB settings)
  - network.json (list of networks)
    - devices.json (list of devices of a network)
      - registers.json (list of registers of a device)

- data format
  - JSON
