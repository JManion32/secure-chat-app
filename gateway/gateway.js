import net from "net";
import { WebSocketServer } from "ws";

const WS_PORT = 8080;
const TCP_HOST = "127.0.0.1";
const TCP_PORT = 5000;

const wss = new WebSocketServer({ port: WS_PORT });

console.log(`[GW] WebSocket listening on :${WS_PORT}`);

wss.on("connection", (ws) => {
  console.log("[GW] Browser connected");

  // One TCP connection per WebSocket
  const tcp = net.createConnection(
    { host: TCP_HOST, port: TCP_PORT },
    () => console.log("[GW] Connected to C++ server")
  );

  /* ---------------- WS → TCP ---------------- */

  ws.on("message", (data) => {
    const json = data.toString("utf8");
    const body = Buffer.from(json, "utf8");

    const frame = Buffer.alloc(4 + body.length);
    frame.writeUInt32BE(body.length, 0);
    body.copy(frame, 4);

    tcp.write(frame);
  });

  /* ---------------- TCP → WS ---------------- */

  let buffer = Buffer.alloc(0);

  tcp.on("data", (chunk) => {
    buffer = Buffer.concat([buffer, chunk]);

    while (buffer.length >= 4) {
      const len = buffer.readUInt32BE(0);
      if (buffer.length < 4 + len) break;

      const json = buffer
        .slice(4, 4 + len)
        .toString("utf8");

      ws.send(json);
      buffer = buffer.slice(4 + len);
    }
  });

  /* ---------------- Cleanup ---------------- */

  const cleanup = () => {
    if (ws.readyState === ws.OPEN) ws.close();
    tcp.destroy();
    console.log("[GW] Connection closed");
  };

  ws.on("close", cleanup);
  ws.on("error", cleanup);
  tcp.on("close", cleanup);
  tcp.on("error", cleanup);
});
