import { useState, useEffect, useRef } from 'react';
import { useNavigate } from 'react-router-dom';
import Message from '../components/Message.tsx';
// import SystemMessage from '../components/SystemMessage.tsx';
import ShopPanel from '../components/ShopPanel.tsx';
import type { ChatMessage } from '../types/ChatMessage.ts';

import '../css/chat.css';

function Chat() {
    const navigate = useNavigate();
    const containerRef = useRef<HTMLDivElement | null>(null);

    const [message, setMessage] = useState("");
    const [messages, setMessages] = useState<ChatMessage[]>([]);

    useEffect(() => {
        const el = containerRef.current;
        if (!el) return;

        el.scrollTop = el.scrollHeight;
    }, [messages]);

    function sendMessage() {
        if (message.trim() === "") return;

        const newMessage = {
            name: "justin",
            content: message,
            sent: true,
        };

        setMessages(prev => [...prev, newMessage]);
        setMessage("");
    }

    return (
        <>
            <div className="chat-page-container">
                <div className={`chat-content-container`}>
                    <div className="navbar-container">
                        <p className="active-count">● 3</p>
                        <button
                            className="default-btn"
                        >
                            Shop (50)
                        </button>
                    </div>
                    <hr className="nav-chat-hr"/>
                    <div className="chat-message-container" ref={containerRef}>
                        {messages.map((msg, index) => (
                            <Message
                                key={index}
                                name={msg.name}
                                content={msg.content}
                                sent={msg.sent}
                            />
                        ))}
                    </div>
                    <div className="input-container">
                        <input
                            type="text"
                            maxLength={512}
                            autoComplete="off"
                            className="chat-input"
                            placeholder="Send chat"
                            value={message}
                            onChange={(e) => setMessage(e.target.value)}
                            onKeyDown={(e) => {
                                if (e.key === "Enter") {
                                    e.preventDefault();
                                    sendMessage();
                                }
                            }}
                        />
                        <a className="send-btn" onClick={sendMessage}>
                            ⌲
                        </a>
                    </div>
                </div>
                <ShopPanel page={false}/>
            </div>
        </>
    );
}

export default Chat;
