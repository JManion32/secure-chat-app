// src/App.tsx
import { BrowserRouter, Route, Routes } from "react-router-dom";
import { useEffect, useRef, useState } from "react";

import Login from "./pages/Login";
import Chat from "./pages/Chat";
import GatewayWS from "./services/gatewayWS";

export default function App() {
    const [messages, setMessages] = useState<any[]>([]);
    const wsRef = useRef<GatewayWS | null>(null);
    const [success, setSuccess] = useState<string>('');
    const [user, setUser] = useState<string>('');
    const [token, setToken] = useState<string>('');
    const [activeCount, setActiveCount] = useState(0);

    useEffect(() => {
        wsRef.current = new GatewayWS("ws://localhost:8080");
        //wsRef.current = new GatewayWS("wss://cchat.fun/ws");

        wsRef.current.onMessage = (msg) => {
            switch (msg.type) {
                case "auth.response":
                    setSuccess(msg.payload.success as string);
                    setUser(msg.payload.name as string);
                    setToken(msg.payload.token as string);
                    break;
                case "chat.response":
                    if (typeof msg.payload.activeCount === "number") {
                        setActiveCount(msg.payload.activeCount);
                    }
                    setMessages(prev => [...prev, msg.payload]);
                    break;
            }
        };

        return () => wsRef.current?.close();
    }, []);

    return (
        <BrowserRouter>
        <Routes>
            <Route
            path="/"
            element={<Login gateway={wsRef} success={success} chosen_name={user}/>}
            />
            <Route
            path="/chat"
            element={<Chat messages={messages} gateway={wsRef} user={user} token={token} activeCount={activeCount} />}
            />
        </Routes>
        </BrowserRouter>
    );
}
