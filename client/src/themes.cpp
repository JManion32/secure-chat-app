#include "../include/themes.hpp"

// ======================
//  MAIN BASE QSS
// ======================
const QString MAIN_QSS = R"(
/* ----------------------------------------------------------
   LOGIN SCREEN - We don't need to change the colors later
-----------------------------------------------------------*/
#login-screen {
    background-color: #1f1f1f;
}

#site-label {
    color: #4CAF50;
    font-size: 48px;
    font-weight: bold;
}

#site-desc {
    color: #4CAF50;
    font-size: 16px;
    font-weight: bold;
}

#username-input {
    margin-top: 18px;
    margin-bottom: 12px;
    border: 1px solid #4CAF50;
    padding: 4px;
}

#connect-button {
    color: #4CAF50;
    border: 1px solid #4CAF50;
    font-weight: bold;
    margin-top: 12px;
    padding: 4px;
}

#connect-button:hover {
    background: #4CAF50;
    color: #000000;
    font-weight: bold;
    margin-top: 12px;
    padding: 4px;
}

/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/
#chat-header {
    padding: 8px 12px;
}

#chat-username {
    font-size: 18px;
    font-weight: bold;
}

#theme-shop-button {
    padding: 8px 12px;
    border-radius: 6px;
}

#message-input-box {
    border-radius: 4px;
    padding: 6px;
}

#chat-content {
    padding: 10px;
}

#message-self, #message-other {
    font-size: 15px;
}

#message-meta-self, #message-meta-other {
    font-size: 11px;
}

#bubble-self {
    border-radius: 10px;
}

#bubble-other {
    border-radius: 10px;
}

#chat-send-button {
    height: 32px;
    border-radius: 8px;
}


/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/
/* Header */
#shop-return-button {
    padding: 8px 12px;
    border-radius: 6px;
}

#shop-credit-label {
    font-size: 18px;
    font-weight: bold;
}

/* Theme Cards */
#theme-card {
    border-radius: 12px;
    padding: 15px;
}

/* Image */
#theme-image {
    margin-bottom: 10px;
}

/* Title */
#theme-title {
    font-size: 16px;
    font-weight: bold;
    margin-bottom: 10px;
}

/* Buy Button */
#theme-buy-button {
    color: white;
    padding: 6px 12px;
    border-radius: 6px;
}

)";

// ======================
//  INDIVIDUAL THEMES
// ======================

const QString THEME_DARK = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #1f1f1f;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #343434;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #454545;
}

#chat-send-button {
    background: #3c6df0;
    color: #ffffff;
}

#chat-send-button:hover {
    background: #4572ed;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #333;
    border: 1px solid #333;
}

#chat-content {
    background: #2A2A2A;
}

#message-input-box {
    background-color: #1e1e1e;
    border: 1px solid #333;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #3c6df0;
}

#bubble-other {
    background: #414141;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #1f1f1f;
}

/* Header */
#shop-return-button {
    background: #343434;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #505050;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #2a2a2a;
    border: 1px solid #3c3c3c;
}

#theme-card:hover {
    border-color: #888;
}

/* Title */
#theme-title {
    color: #ffffff;
}

/* Buy Button */
#theme-buy-button {
    background: #0078ff;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #3399ff;
}
)";

const QString THEME_LIGHT = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #ffffff;
}

#chat-username {
    color: #222222;
}

#theme-shop-button {
    background: #DDDDDD;
    color: #222222;
}

#theme-shop-button:hover {
    background: #CCCCCC;
}

#chat-send-button {
    background: #446fe5;
    color: #FFFFFF;
}

#chat-send-button:hover {
    background: #3664e3;
}

#chat-scroll QScrollBar:vertical {
    background: #CCCCCC;
    color: #CCCCCC;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #AAAAAA;
}

#chat-content {
    background: #ffffff;
}

#message-input-box {
    background: #DDDDDD;
    color: #222222;
}

#bubble-self {
    background: #446fe5;
}

#bubble-other {
    background: #DDDDDD;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #222222;
}

#message-meta-self, #message-meta-other {
    color: #888888;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #ffffff;
}

/* Header */
#shop-return-button {
    background: #DDDDDD;
    color: #222222;
}

#shop-return-button:hover {
    background: #CCCCCC;
}

#shop-credit-label {
    color: #222222;
}

/* Theme Cards */
#theme-card {
    background: #DDDDDD;
    border: 1px solid #CCC;
}

#theme-card:hover {
    border-color: #999;
}

/* Title */
#theme-title {
    color: #222222;
}

/* Buy Button */
#theme-buy-button {
    background: #0078ff;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #3399ff;
}
)";

const QString THEME_NEON = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #1f1f1f;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #FF1199;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #ff36a8;
}

#chat-send-button {
    background: #11FFEE;
    color: #000000;
}

#chat-send-button:hover {
    background: #6bfff5;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #333;
    border: 1px solid #333;
}

#message-input-box {
    background-color: #1e1e1e;
    border: 1px solid #333;
}

#message-self {
    color: #000000;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #11FFEE;
}

#bubble-other {
    background: #FF1199;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #1f1f1f;
}

/* Header */
#shop-return-button {
    background: #343434;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #505050;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #2a2a2a;
    border: 1px solid #3c3c3c;
}

#theme-card:hover {
    border-color: #FF1199;
}

/* Title */
#theme-title {
    color: #ffffff;
}

/* Buy Button */
#theme-buy-button {
    background: #11FFEE;
    color: #000000;
}

#theme-buy-button:hover {
    background: #7dfff6;
}

)";

const QString THEME_WARM = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #1f1f1f;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #cf6016;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #cc6e2f;
}

#chat-send-button {
    background: #ff0000;
    color: #ffffff;
}

#chat-send-button:hover {
    background: #fd3535;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #333;
    border: 1px solid #333;
}

#message-input-box {
    background-color: #4a4a4a;
    border: 1px solid #333;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #222222;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #ff0000;
}

#bubble-other {
    background: #e2e205;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #1f1f1f;
}

/* Header */
#shop-return-button {
    background: #ffa407;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #cc6e2f;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #2a2a2a;
    border: 1px solid #3c3c3c;
}

#theme-card:hover {
    border-color: #ffa407;
}

/* Title */
#theme-title {
    color: #ffffff;
}

/* Buy Button */
#theme-buy-button {
    background: #b30000;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #d90000;
}

)";

const QString THEME_PREMIUM = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #1f1f1f;
}

#chat-username {
    color: #ffbf00;
}

#theme-shop-button {
    background: #a67c00;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #bf9b30;
}

#chat-send-button {
    background: #ffcf40;
    color: #111111;
}

#chat-send-button:hover {
    background: #ffd863;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #a67c00;
    border: 1px solid #333;
}

#chat-content {
    background: #2A2A2A;
}

#message-input-box {
    background-color: #1e1e1e;
    border: 1px solid #333;
}

#message-self {
    color: #111111;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #ffcf40;
}

#bubble-other {
    background: #bf9b30;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #1f1f1f;
}

/* Header */
#shop-return-button {
    background: #a67c00;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #bf9b30;
}

#shop-credit-label {
    color: #ffbf00;
}

/* Theme Cards */
#theme-card {
    background: #2a2a2a;
    border: 1px solid #a67c00;
}

#theme-card:hover {
    border-color: #bf9b30;
}

/* Title */
#theme-title {
    color: #ffbf00;
}

/* Buy Button */
#theme-buy-button {
    background: #a67c00;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #bf9b30;
}

)";

const QString THEME_RETRO = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #666547
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #ffe28a;
    color: #222222;
}

#theme-shop-button:hover {
    background: #ffecb4;
}

#chat-send-button {
    background: #ffe28a;
    color: #222222;
}

#chat-send-button:hover {
    background: #ffecb4;
}

#chat-scroll QScrollBar:vertical {
    background: #CCCCCC;
    color: #CCCCCC;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #AAAAAA;
}

#chat-content {
    background: #fffeb3;
}

#message-input-box {
    background: #DDDDDD;
    color: #222222;
}

#bubble-self {
    background: #6fcb9f;
}

#bubble-other {
    background: #fb2e01;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #888888;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #666547
}

/* Header */
#shop-return-button {
    background: #ffe28a;
    color: #222222;
}

#shop-return-button:hover {
    background: #ffecb4;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #ffe28a;
    border: 1px solid #666547;
}

#theme-card:hover {
    border-color: #666547;
}

/* Title */
#theme-title {
    color: #222222;
}

/* Buy Button */
#theme-buy-button {
    background: #6fcb9f;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #7ed9ad;
}
)";

const QString THEME_FOREST = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #19270d;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #72601b;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #85711f;
}

#chat-send-button {
    background: #818c3c;
    color: #ffffff;
}

#chat-send-button:hover {
    background: #9ba844;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #333;
    border: 1px solid #333;
}

#chat-content {
    background: #593a0e;
}

#message-input-box {
    background-color: #1e1e1e;
    border: 1px solid #333;
}

#message-self, #message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #818c3c;
}

#bubble-other {
    background: #25591f;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #19270d;
}

/* Header */
#shop-return-button {
    background: #72601b;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #85711f;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #593a0e;
    border: 1px solid #72601b;
}

#theme-card:hover {
    border-color: #72601b;
}

/* Title */
#theme-title {
    color: #ffffff;
}

/* Buy Button */
#theme-buy-button {
    background: #25591f;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #2b6924;
}

)";

const QString THEME_HALLOWEEN = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #000000;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #fd9702;
    color: #000000;
}

#theme-shop-button:hover {
    background: #ffb13b;
}

#chat-send-button {
    background: #e102ff;
    color: #ffffff;
}

#chat-send-button:hover {
    background: #e837ff;
}

#chat-scroll QScrollBar:vertical {
    background: #1A1A1A;
    color: #1A1A1A;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #ff7100;
    border: 1px solid #333;
}

#chat-content {
    background: #000000;
    border: 1px solid #ff7100;
}

#message-input-box {
    background-color: #1e1e1e;
    border: 1px solid #ff7100;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #cfcfcf;
}

#bubble-self {
    background: #ae03ff;
}

#bubble-other {
    background: #ff7100;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #000000;
}

/* Header */
#shop-return-button {
    background: #fd9702;
    color: #000000;
}

#shop-return-button:hover {
    background: #ffb13b;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #000000;
    border: 1px solid #ff7100;
}

#theme-card:hover {
    border-color: #fd9702;
}

/* Title */
#theme-title {
    color: #ffffff;
}

/* Buy Button */
#theme-buy-button {
    background: #e102ff;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #e837ff;
}

)";

const QString THEME_FESTIVE = R"(
/* ----------------------------------------
   CHAT ROOM
-----------------------------------------*/

#chat-screen {
    background-color: #075600;
}

#chat-username {
    color: #ffffff;
}

#theme-shop-button {
    background: #ff0000;
    color: #ffffff;
}

#theme-shop-button:hover {
    background: #ff4f4f;
}

#chat-send-button {
    background: #ff5454;
    color: #FFFFFF;
}

#chat-send-button:hover {
    background: #ff6c6c;
}

#chat-scroll QScrollBar:vertical {
    background: #CCCCCC;
    color: #CCCCCC;
}

#chat-scroll QScrollBar::handle:vertical {
    background: #AAAAAA;
}

#chat-content {
    background: #ffffff;
}

#message-input-box {
    background: #DDDDDD;
    color: #222222;
}

#bubble-self {
    background: #ff7878;
}

#bubble-other {
    background: #74d680;
}

#message-self {
    color: #ffffff;
}

#message-other {
    color: #ffffff;
}

#message-meta-self, #message-meta-other {
    color: #888888;
}

/* ----------------------------------------
   SHOP SCREEN STYLING
-----------------------------------------*/

#shop-screen {
    background-color: #075600;
}

/* Header */
#shop-return-button {
    background: #ff0000;
    color: #ffffff;
}

#shop-return-button:hover {
    background: #ff4f4f;
}

#shop-credit-label {
    color: #ffffff;
}

/* Theme Cards */
#theme-card {
    background: #ffffff;
    border: 1px solid #74d680;
}

#theme-card:hover {
    border-color: #ff0000;
}

/* Title */
#theme-title {
    color: #222222;
}

/* Buy Button */
#theme-buy-button {
    background: #720000;
    color: #ffffff;
}

#theme-buy-button:hover {
    background: #920000;
}
)";

// ======================
//  THEME MAP
// ======================

const QHash<QString, QString> THEME_MAP = {
    { "dark",      THEME_DARK },
    { "light",     THEME_LIGHT },
    { "warm",      THEME_WARM },
    { "neon",      THEME_NEON },
    { "forest",    THEME_FOREST },
    { "retro",     THEME_RETRO },
    { "halloween", THEME_HALLOWEEN },
    { "festive",   THEME_FESTIVE },
    { "premium",   THEME_PREMIUM }
};
