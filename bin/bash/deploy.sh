#!/bin/bash
set -e

# Always run from the frontend directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FRONTEND_DIR="$SCRIPT_DIR/../../client_site"

echo "▶ Changing to frontend directory: $FRONTEND_DIR"
cd "$FRONTEND_DIR"

echo "▶ Building frontend..."
npm run build

echo "▶ Deploying to /srv/chat-site..."
sudo rsync -av --delete dist/ /srv/chat-site/

echo "✅ Deployment complete"
