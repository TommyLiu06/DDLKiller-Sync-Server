# Sync Server for DDLKiller

## Deployment

### 1. Install Requirements

```bash
# 0. Update system
sudo apt update && sudo apt upgrade

# 1. Development tools
sudo apt install build-essential cmake

# 2. Boost
sudo apt install libboost-all-dev

# 3. SQLite3
sudo apt install sqlite3 libsqlite3-dev

# 4. SQLiteCpp
sudo apt install libsqlitecpp-dev

# 5. nlohmann-json
sudo apt install nlohmann-json3-dev
```

### 2. Build

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

### 3. Run Server

```bash
./ws_server <port> <database_path>
```

example:

```bash
./ws_server 8090 /opt/ddl_sync_server/ddl_sync.db
```

### 4. Systemd Service (optional)

```bash
sudo cp ../ddl_sync_server.service /etc/systemd/system/ddl_sync_server.service
sudo systemctl daemon-reload
sudo systemctl enable ddl_sync_server.service
sudo systemctl start ddl_sync_server.service
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

### 2. Run test
```bash
wscat -c ws://localhost:<port>
```
