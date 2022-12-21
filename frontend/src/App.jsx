import { useState, useEffect } from "react";
import ProgressRing from "./components/ProgressRing";
import "./App.sass";

import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, set } from "firebase/database";

import imgDay from "./assets/day.png";
import imgNight from "./assets/night.png";
import imgAlt from "./assets/altitude.png";

const firebaseConfig = {
  databaseURL: "https://iot-qamd-default-rtdb.firebaseio.com",
};
const app = initializeApp(firebaseConfig);
const db = getDatabase();

// SET
/*
  set(ref(db, "users/" + formData.name), {
      username: formData.name,
      email: formData.email,
      message: formData.message,
      time: dateTimeNow(),
    });
 */

// GET

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
      <header className="flex flex-col items-center">
        <h1 className="text-4xl text-gray-600">Cairo, Egypt</h1>
        <p className="mt-2 text-gray-600">21/05/2022</p>
      </header>
      <main className="flex items-center gap-5 mt-5">
        <section>
          <h1 className="text-8xl font-semibold text-gray-800">
            {temp}&#8451;
          </h1>
        </section>
        <img className="w-[250px] h-[250px] scale-x-[-1]" src={imgDay} />
      </main>
      <footer className="flex justify-between items-center mt-5">
        <div>
          <ProgressRing percentage={hum} />
          <p className="mt-3">Humidity</p>
        </div>
        <div className="flex items-center">
          <img className="w-9 h-9" src={imgAlt} />
          <p>{alt} m</p>
        </div>
      </footer>
    </div>
  );
}

export default App;
