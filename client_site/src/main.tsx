import { BrowserRouter, Route, Routes } from 'react-router-dom';
import { createRoot } from 'react-dom/client'

import './css/index.css'
import './css/colors.css'

import Login from './pages/Login.tsx'
import Chat from './pages/Chat.tsx'

createRoot(document.getElementById('root')!).render(
      <BrowserRouter>
        <Routes>
            <Route path="/" element={<Login/>} />
            <Route path="/Chat" element={<Chat/>} />
        </Routes>
    </BrowserRouter>
)
