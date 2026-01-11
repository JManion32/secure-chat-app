// src/App.tsx
import { BrowserRouter, Route, Routes } from "react-router-dom";
import { useEffect, useRef, useState } from "react";

import Login from "./pages/Login";
import Chat from "./pages/Chat";
import GatewayWS from "./services/gatewayWS";

export default function App() {
    const [messages, setMessages] = useState<any[]>([]);
    const wsRef = useRef<GatewayWS | null>(null);
    const [user, setUser] = useState<string>('');
    const [token, setToken] = useState<string>('');

    useEffect(() => {
        wsRef.current = new GatewayWS("ws://localhost:8080");

        wsRef.current.onMessage = (msg) => {
        switch (msg.type) {
            case "auth.response":
                setUser(msg.payload.name as string);
                setToken(msg.payload.token as string);
                break;
            case "chat.response":
                setMessages(prev => [...prev, msg.payload]);
                break;
        }
        };

        return () => wsRef.current?.close();
    }, []);

    useEffect(() => {
        console.log("USER UPDATED:", user);
    }, [user]);

    return (
        <BrowserRouter>
        <Routes>
            <Route
            path="/"
            element={<Login gateway={wsRef} />}
            />
            <Route
            path="/chat"
            element={<Chat messages={messages} gateway={wsRef} user={user} token={token} />}
            />
        </Routes>
        </BrowserRouter>
    );
}
