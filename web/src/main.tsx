import React from 'react'
import ReactDOM from 'react-dom/client'
import App from './App'
import './index.css'
import Skeletal from "./Skeletal";
import {BrowserRouter, Route, Routes} from "react-router-dom";

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
    <React.StrictMode>
        <BrowserRouter basename={"/msc-bioinformatyka-2022"}>
            <Routes>
                <Route path={"/index.html"} element={<App />} />
                <Route path={"/skeletal.html"} element={<Skeletal />} />
            </Routes>
        </BrowserRouter>
    </React.StrictMode>
)
