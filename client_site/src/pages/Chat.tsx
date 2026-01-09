import { useState, useEffect, useRef } from 'react';
import Message from '../components/Message.tsx';
import ShopModal from '../components/ShopModal.tsx';
import SystemMessage from '../components/SystemMessage.tsx';
import ShopPanel from '../components/ShopPanel.tsx';
import type { ChatMessage } from '../types/ChatMessage.ts';
import logo from '../assets/logo.svg';

import '../css/chat.css';

function Chat() {
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

    function goToPortfolio() {
        window.location.href = 'https://justinmanion.com/projects/securechatroom';
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
                        <p className="active-count"><span>Active: </span>3</p>
                        <div className="shop-btn-container">
                            <ShopModal/>
                        </div>
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
                        <Message
                            name="George Washington"
                            content="The unanimous Declaration of the thirteen united States of America, When in the Course of human events, it becomes necessary for one people to dissolve the political bands which have connected them with another, and to assume among the powers of the earth, the separate and equal station to which the Laws of Nature and of Nature's God entitle them, a decent respect to the opinions of mankind requires that they should declare the causes which impel them to the separation."
                            sent={false}
                        />
                        <SystemMessage content="George Washington left the room"/>
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
