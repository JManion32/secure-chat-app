#!/bin/bash
set -e

echo "▶ Building frontend..."
npm run build

echo "▶ Deploying to /srv/chat-site..."
sudo rsync -av --delete dist/ /srv/chat-site/

echo "✅ Deployment complete"
