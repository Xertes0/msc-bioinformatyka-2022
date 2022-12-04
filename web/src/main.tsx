import React from 'react'
import ReactDOM from 'react-dom/client'
import {createBrowserRouter, RouterProvider,} from "react-router-dom";
import App from './App'
import './index.css'
import SkeletalFormula from "./react_components/SkeletalFormula/SkeletalFormula";

const router = createBrowserRouter([
    {
        path: "/msc-bioinformatyka-2022/",
        element: <App/>,
    },
    {
        path: "/msc-bioinformatyka-2022/skeletal/:formula",
        element: <SkeletalFormula/>
    }
]);

ReactDOM.createRoot(document.getElementById('root') as HTMLElement).render(
    <React.StrictMode>
        <RouterProvider router={router}/>
    </React.StrictMode>
)
