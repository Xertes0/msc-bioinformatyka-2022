import React from "react";
import ReactDOM from "react-dom/client";
import { BrowserRouter, Route, Routes } from "react-router-dom";

import "./index.css";
import App from "./App";
import Skeletal from "./Skeletal";

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
  <React.StrictMode>
    <BrowserRouter basename={"/msc-bioinformatyka-2022"}>
      <Routes>
        <Route path={"/"}>
          <Route index element={<App />} />
          <Route path={"skeletal.html"} element={<Skeletal />} />
        </Route>
      </Routes>
    </BrowserRouter>
  </React.StrictMode>
);
