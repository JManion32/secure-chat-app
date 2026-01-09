//import { useEffect, useState } from 'react';

import '../css/shop.css';
import '../css/colors.css';
import '../css/index.css';

function ShopPanel() {

    return (
        <>
            <div className={`shop-container`}>
                <div className="shop-nav">
                    <h2 className="shop-title">Shop</h2>
                    <p className="token-count">Tokens: 50</p>
                </div>
                <hr/>
                <div>
                    <h2>Themes</h2>
                    <div className="coming-soon">
                        Coming Soon!
                    </div>
                </div>
                <hr/>
                <div>
                    <h2>Banners</h2>
                    <div className="coming-soon">
                        Coming Soon!
                    </div>
                </div>
            </div>
        </>
    );
}

export default ShopPanel;