export type BaseMessage<T = any> = {
  type: string;
  payload: T;
};

export type AuthRequest = BaseMessage<{
  name: string;
}>;

export type ChatSend = BaseMessage<{
  content: string;
}>;

export type ServerMessage =
  | BaseMessage<{ content: string }>
  | BaseMessage<{ credits: number }>
  | BaseMessage<any>;
