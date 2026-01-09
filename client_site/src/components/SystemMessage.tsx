//import { useState } from 'react';
import '../css/message.css';

type Props = {
    content: string;
};

function SystemMessage({ content }: Props) {
    return (
        <>
            <p className="system-message">{content}</p>
        </>
    );
}

export default SystemMessage;