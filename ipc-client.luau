local ipc_client = {}
      ipc_client.LOAD_SO = true
      ipc_client.SO_PREFIX = "vlm_"

function ipc_client.connect_socket() end
function ipc_client.send() end
function ipc_client.recv() end
function ipc_client.close() end

return ipc_client