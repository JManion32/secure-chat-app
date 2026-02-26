import { useState, useEffect, useRef } from 'react';
import GatewayWS from "../services/gatewayWS";

import Message from '../components/Message.tsx';
import ShopModal from '../components/ShopModal.tsx';
import ShopPanel from '../components/ShopPanel.tsx';
import type { ChatMessage } from '../types/ChatMessage.ts';
import logo from '../assets/logo.svg';

import '../css/chat.css';

type ChatProps = {
    messages: ChatMessage[];
    gateway: GatewayWS;
    user: string;
    token: string;
    activeCount: number;
};

function Chat({ messages, gateway, user, token, activeCount }: ChatProps) {
    const containerRef = useRef<HTMLDivElement | null>(null);
    const [message, setMessage] = useState("");

    // Auto-scroll when messages update
    useEffect(() => {
        const el = containerRef.current;
        if (!el) return;
        el.scrollTop = el.scrollHeight;
    }, [messages]);

    function sendMessage() {
        if (message.trim() === "") return;

        gateway.send({
            type: "chat.request",
            payload: {
                content: message,
                token: token
            }
        });

        setMessage("");
    }

    function goToPortfolio() {
        window.location.href = "https://justinmanion.com/projects/cchat";
    }

    return (
        <>
            <div className="chat-page-container">
                <div className={`chat-content-container`}>
                    <div className="navbar-container">
                        <div className="cchat-link-container" onClick={ () => goToPortfolio() }>
                            <img src={logo} className="site-logo"/>
                            <p className="site-name">CChat</p>
                        </div>
                        <button className="count">
                            <span className="active-dot">●</span> <span className="active-count">{activeCount}</span>
                        </button>
                        <div className="shop-btn-container">
                            <ShopModal/>
                        </div>
                    </div>
                    <hr className="nav-chat-hr"/>
                    <div className="chat-message-container" ref={containerRef}>
                        {messages.map((msg, index) => (
                            <Message
                                key={index}
                                server={msg.server}
                                name={msg.name}
                                content={msg.content}
                                sent={msg.name === user}
                            />
                        ))}
                    </div>
                    <div className="input-container">
                        <input
                            type="text"
                            maxLength={512}
                            autoComplete="off"
                            className="chat-input"
                            placeholder={`Send message as ${user}`}
                            value={message}
                            onChange={(e) => setMessage(e.target.value)}
                            onKeyDown={(e) => {
                                if (e.key === "Enter") {
                                    e.preventDefault();
                                    sendMessage();
                                }
                            }}
                        />
                        <a className={`send-btn ${message !== '' ? '' : 'disabled'}`} onClick={sendMessage}>
                            ⌲
                        </a>
                    </div>
                </div>
                <div className="shop-panel-container">
                    <ShopPanel/>
                </div>
            </div>
        </>
    );
}

export default Chat;
