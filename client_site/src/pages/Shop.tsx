//import { useEffect, useState } from 'react';
//import { useNavigate } from 'react-router-dom';
import ShopPanel from '../components/ShopPanel.tsx'

import '../css/shop.css';
import '../css/colors.css';
import '../css/index.css';

function Shop() {
    //const navigate = useNavigate();

    return (
        <>
            <div className={`shop-page-container`}>
                <ShopPanel page={false}/>
            </div>
        </>
    );
}

export default Shop;