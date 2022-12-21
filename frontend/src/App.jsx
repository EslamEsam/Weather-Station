import { useState, useEffect } from "react";
import "./App.sass";
import Home from "./components/Home";
import Admin from "./components/Admin";

import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, set } from "firebase/database";

import { Routes, Route } from "react-router-dom";

import imgDay from "./assets/day.png";
import imgNight from "./assets/night.png";
import imgAlt from "./assets/altitude.png";

const firebaseConfig = {
  databaseURL: "https://iot-qamd-default-rtdb.firebaseio.com",
};
const app = initializeApp(firebaseConfig);
const db = getDatabase();

function App() {
  const [temp, setTemp] = useState(0);
  const [alt, setAlt] = useState(0);
  const [hum, setHum] = useState(0);

  useEffect(() => {
    const temp = ref(db, "LDR/temp"),
      alt = ref(db, "LDR/alt"),
      hum = ref(db, "LDR/hum");

    onValue(temp, (snapshot) => {
      const data = snapshot.val();
      setTemp(data);
    });

    onValue(alt, (snapshot) => {
      const data = snapshot.val();
      setAlt(data);
    });

    onValue(hum, (snapshot) => {
      const data = snapshot.val();
      setHum(data);
    });
  }, []);

  return (
    <div className="App h-[100vh] grid place-content-center">
      {/* Routing */}
      <Routes>
        <Route
          path="/"
          element={
            <Home
              imgDay={imgDay}
              imgAlt={imgAlt}
              temp={temp}
              alt={alt}
              hum={hum}
            />
          }
        />
        <Route path="/admin" element={<Admin />} />
      </Routes>
    </div>
  );
}

export default App;
