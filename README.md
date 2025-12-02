# Sync Server for DDLKiller

## Deployment

### 1. Install Requirements

```bash
sudo apt update && sudo apt upgrade

sudo apt install build-essential cmake libboost-all-dev sqlite3 libsqlite3-dev libsqlitecpp-dev nlohmann-json3-dev
```

If some packages are not found, you may need to install the from source.

### 2. Build

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

NOTICE: If you are building on devices with low memory (<=2GB), consider using `make -j1` or using swap to avoid out-of-memory errors.

### 3. Run Server

```bash
./ddl_sync_server <port> <database_path>
```

example:

```bash
./ddl_sync_server 8090 /opt/ddl_sync_server/ddl_sync.db
```

### 4. Config Systemd Service (optional)

Run:

```bash
cd ..
sudo ./install_service.sh <port>
```

The service file will be created at `/etc/systemd/system/ddl_sync_server.service`.

The binary file will be copied to `/opt/ddl_sync_server/ddl_sync_server`.

The default database path is `/opt/ddl_sync_server/ddl_sync.db`.

Then enable and start the service:

```bash
sudo systemctl enable ddl_sync_server
sudo systemctl start ddl_sync_server
```

## Local test

### 0. Install `Node.js` and `npm` (if not installed) 

```bash
sudo apt install nodejs npm
```

### 1. Install `wscat`

```bash
npm install wscat
```

### 2. Connect Server

```bash
wscat -c ws://localhost:<port>
```

### 3. Send Test Message

```json
{"type":"modification","content":{"operation":"add","uuid":"20240615093000123","title":"Example Title","description":"This is an example description.","due_date":"2024-12-31"}}
```

and you should see the response messages:

On source client:
```json 
{"content":{"status":"success"},"type":"response"}
```

On other clients:

```json
{"type":"modification","content":{"operation":"add","uuid":"20240615093000123","title":"Example Title","description":"This is an example description.","due_date":"2024-12-31"}}
```
