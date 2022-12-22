import { useState, useEffect } from "react";
import "./App.sass";
import Home from "./components/Home";
import Admin from "./components/Admin";

import { Routes, Route } from "react-router-dom";

import imgDay from "./assets/day.png";
import imgAlt from "./assets/altitude.png";



function App() {
  return (
    <div className="App h-[100vh] grid place-content-center">
      <Routes>
        <Route
          path="/"
          element={
            <Home/>
          }
        />
        <Route path="/admin" element={<Admin />} />
      </Routes>
    </div>
  );
}

export default App;
