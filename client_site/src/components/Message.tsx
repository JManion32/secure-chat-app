//import { useState } from 'react';
import '../css/message.css';

type Props = {
    server: string;
    name: string;
    content: string;
    date?: string;
    sent: boolean;
};

function Message({ server, name, content, sent }: Props) {
    return (
        <>
            <div className={`message-container ${server === 'true' ? 'hide' : ''} ${sent ? 'sent-message' : 'received-message'}`}>
                <div className={`message-bubble ${sent ? 'sent-bubble' : 'received-bubble'}`}>
                    <p className="message-bubble-p">{content}</p>
                </div>
                <div className={`message-info ${sent ? 'sent-info' : 'received-info'}`}>
                    <span>{sent ? '' : name}</span>
                </div>
            </div>
            <div className={` system-message-container ${server === 'true' ? '' : 'hide'}`}>
                <p className="system-message">{content}</p>
            </div>
        </>
    );
}

export default Message;