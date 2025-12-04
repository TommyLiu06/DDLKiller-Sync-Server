#!/usr/bin/env bash
set -euo pipefail

# install_service.sh
# Usage: sudo ./install_service.sh <port>
#
# What it does:
#  - Checks that build/ddl_sync_server exists
#  - Aborts if /opt/ddl_sync_server or /etc/systemd/system/ddl_sync_server.service already exist
#  - Creates /opt/ddl_sync_server and copies the executable there
#  - Uses default database path: /opt/ddl_sync_server/ddl_sync.db
#  - Writes a systemd service file /etc/systemd/system/ddl_sync_server.service
#    The service uses environment variable DDL_SYNC_PORT to pass the port
#    and DDL_SYNC_DB for the database path to the executable.
#  - Runs systemctl daemon-reload
#  - Prints instructions to enable/start the service

usage() {
  cat <<EOF
Usage: $0 <port>
Example:
  sudo $0 8090

This script defaults the database path to: /opt/ddl_sync_server/ddl_sync.db
EOF
  exit 1
}

if [ "$#" -ne 1 ]; then
  usage
fi

PORT="$1"
DB_PATH="/opt/ddl_sync_server/ddl_sync.db"

# require root because we write to /opt and /etc/systemd
if [ "$(id -u)" -ne 0 ]; then
  echo "Error: this script must be run as root (use sudo)."
  exit 1
fi

SRC="./build/ddl_sync_server"
DEST_DIR="/opt/ddl_sync_server"
SERVICE_FILE="/etc/systemd/system/ddl_sync_server.service"

# Validate source
if [ ! -f "$SRC" ]; then
  echo "Error: $SRC does not exist. Please build the project first (see README)."
  exit 1
fi

# Validate destination directory and service file do not already exist
if [ -e "$DEST_DIR" ]; then
  echo "Error: $DEST_DIR already exists. Aborting to avoid overwriting." >&2
  exit 1
fi

if [ -e "$SERVICE_FILE" ]; then
  echo "Error: $SERVICE_FILE already exists. Aborting to avoid overwriting." >&2
  exit 1
fi

# Validate port
if ! printf '%s' "$PORT" | grep -Eq '^[0-9]+$'; then
  echo "Error: port must be a number." >&2
  exit 1
fi
if [ "$PORT" -lt 1 ] || [ "$PORT" -gt 65535 ]; then
  echo "Error: port must be between 1 and 65535." >&2
  exit 1
fi

# Create destination and copy the binary
mkdir -p "$DEST_DIR"
cp -p "$SRC" "$DEST_DIR/ddl_sync_server"
chmod 755 "$DEST_DIR/ddl_sync_server"

# Ensure parent directory for DB exists (we already created DEST_DIR)
# Create an empty DB file if it doesn't exist to avoid runtime surprises
if [ ! -e "$DB_PATH" ]; then
  touch "$DB_PATH"
  chown root:root "$DB_PATH" || true
  chmod 644 "$DB_PATH" || true
fi

# Create systemd service file using environment variables for port and db
cat > "$SERVICE_FILE" <<EOF
[Unit]
Description=DDL Sync Server
After=network.target

[Service]
Type=simple
Environment="DDL_SYNC_PORT=${PORT}"
Environment="DDL_SYNC_DB=${DB_PATH}"
ExecStart=${DEST_DIR}/ddl_sync_server \${DDL_SYNC_PORT} \${DDL_SYNC_DB}
Restart=always
RestartSec=5s
WorkingDirectory=${DEST_DIR}
User=root
StandardOutput=journal
StandardError=journal
SyslogIdentifier=ddl_sync_server

[Install]
WantedBy=multi-user.target
EOF

# Reload systemd to pick up new unit
systemctl daemon-reload

cat <<EOF
Installation complete.
Service file written to: $SERVICE_FILE
Binary copied to: $DEST_DIR/ddl_sync_server
Default database path: $DB_PATH

Next steps (run as root or with sudo):
  sudo systemctl enable ddl_sync_server    # enable at boot
  sudo systemctl start ddl_sync_server     # start now
  sudo systemctl status ddl_sync_server    # check status

If you want this script to also enable/start the service automatically, I can add flags for that.
EOF

exit 0
