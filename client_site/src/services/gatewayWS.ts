import type { BaseMessage } from "../types/protocol";

type MessageHandler = (msg: BaseMessage) => void;

export default class GatewayWS {
  private ws: WebSocket;
  public onMessage?: MessageHandler;

  constructor(url: string) {
    this.ws = new WebSocket(url);

    this.ws.onopen = () => {
      console.log("[WS] connected");
    };

    this.ws.onmessage = (event: MessageEvent) => {
      const msg: BaseMessage = JSON.parse(event.data);
      this.onMessage?.(msg);
    };

    this.ws.onerror = (err) => {
      console.error("[WS] error", err);
    };

    this.ws.onclose = () => {
      console.log("[WS] closed");
    };
  }

  send<T>(msg: BaseMessage<T>) {
    if (this.ws.readyState !== WebSocket.OPEN) return;
    this.ws.send(JSON.stringify(msg));
  }

  close() {
    this.ws.close();
  }
}
