import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';

import '../css/login.css';
import '../css/colors.css';
import '../css/index.css';
import { badName } from '../utils/checkName.ts';
import MatrixCanvas from '../components/MatrixCanvas.tsx';

type LoginProps = {
    gateway: React.MutableRefObject<any>;
    success: string;
    chosen_name: string;
};

function Login({ gateway, success, chosen_name }: LoginProps) {
    const navigate = useNavigate();
    const [badNameMessage, setBadNameMessage] = useState('');
    const [joinAttempt, setJoinAttempt] = useState(0);

    const [copied, setCopied] = useState(false);
    const handleCopy = () => {
        navigator.clipboard.writeText('https://cchat.fun');
        setCopied(true);
        setTimeout(() => setCopied(false), 3000);
    };

    const [name, setName] = useState(() => {
        return localStorage.getItem('savedName') ?? '';
    });

    useEffect(() => {
        localStorage.setItem('savedName', name);
    }, [name]);

    function start() {
        if (badName(name)) {
            setName('');
            setBadNameMessage('Please choose a clean username!');
            return;
        }

        setBadNameMessage('');

        setJoinAttempt(a => a + 1);

        gateway.current?.send({
            type: "auth.request",
            payload: { name }
        });
    }


    useEffect(() => {
        if (joinAttempt === 0) return;

        if (success === "false") {
            setBadNameMessage(`The username ${chosen_name} is taken.`);
            setName('');
        }

        if (success === "true") {
            navigate("/chat");
        }
    }, [success, joinAttempt]);


    return (
        <>
            <div className="home-page-container">
                <MatrixCanvas/>
                <div className="home-content-container">
                    <h1 className="site-title">
                        cchat.fun
                    </h1>
                    <p className={`name-error`}>
                        {badNameMessage}
                    </p>
                    <input
                        type="text"
                        maxLength={16}
                        autoComplete="off"
                        id="name-input"
                        className={`name-input`}
                        placeholder="Enter Username"
                        title="Enter your username"
                        value={name}
                        onChange={(e) => setName(e.target.value)}
                    />
                    <div className="start-btns-container">
                        <button
                            className={`start-btn ${name == '' ? 'disabled' : ''}`}
                            id="find-match-btn"
                            onClick={() => start()}
                        >
                            Join
                        </button>
                    </div>
                </div>
                <div className="home-footer-container">
                    <a className="footer-link" onClick={handleCopy}>
                        {copied ? 'Copied!' : 'Share'}
                    </a>
                    <span>|</span>
                    <a
                        className="footer-link"
                        href="https://github.com/JManion32/secure-chat-app"
                        target="_blank"
                    >
                        GitHub ↗
                    </a>
                    <span>|</span>
                    <a
                        className="footer-link"
                        href="https://justinmanion.com/projects/securechatroom"
                        target="_blank"
                    >
                        Portfolio ↗
                    </a>
                </div>
            </div>
        </>
    );
}

export default Login;
