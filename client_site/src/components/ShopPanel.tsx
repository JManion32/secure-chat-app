//import { useEffect, useState } from 'react';
import { useNavigate } from 'react-router-dom';

import '../css/shop.css';
import '../css/colors.css';
import '../css/index.css';

type Props = {
    page: boolean;
};

function ShopPanel({ page }: Props) {
    const navigate = useNavigate();

    return (
        <>
            <div className={`shop-container ${page ? '' : 'shop-panel-container'}`}>
                <div className="shop-nav">
                    <button className="default-btn chat-btn" onClick={ () => navigate('/chat')}>
                        ⬅ Chat
                    </button>
                    <p className="token-count">Tokens: 50</p>
                </div>
                <hr/>
            </div>
        </>
    );
}

export default ShopPanel;