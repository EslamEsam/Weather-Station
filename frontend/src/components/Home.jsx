import ProgressRing from "./ProgressRing";

import imgAltitude from '../assets/altitude.png';
import imgDay from '../assets/day.png';
import imgPressure from '../assets/pressure.png';
import imgHumidity from '../assets/humidity.png';
import { useState, useEffect } from "react";


import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue } from "firebase/database";

const firebaseConfig = {
  databaseURL: "https://iot-qamd-default-rtdb.firebaseio.com",
};
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);


export default function Home({}) {
  const [reads, setReads] = useState({
    temp: 0,
    altitude: 0,
    pressure: 0,
    humidity: 0
  })

  useEffect(() => {
    
    const fetchedReads = {
      temp: ref(db, "reads/temp"),
      alt: ref(db, "reads/alt"),
      hum: ref(db, "reads/hum"),
      pres: ref(db, "reads/pres")
    }

    for(const read in fetchedReads){
      onValue(fetchedReads[read], (snapshot) => {
        const data = snapshot.val();
        console.log(data);
        setReads((old) => {
          return {...old, [read]: data};
        })
      });
    }
  }, []);

  return (
    <>
      <header className="flex flex-col items-center">
        <h1 className="text-4xl text-gray-600">Cairo, Egypt</h1>
        <p className="mt-2 text-gray-600">21/05/2022</p>
      </header>
      
      <main className="flex items-center gap-5 mt-5">
        <section>
          <h1 className="text-8xl font-semibold text-gray-800">
            {reads.temp}&#8451;
          </h1>
        </section>
        <img className="w-[250px] h-[250px] scale-x-[-1]" src={imgDay} />
      </main>
      
      <footer className="flex justify-between items-end mt-5">
        <div>
          <ProgressRing percentage={reads.hum} />
          <p className="mt-3">Humidity</p>
        </div>

        <div className="flex flex-col items-center gap-2">
          <div className="flex items-center gap-3">
            <img className="w-9 h-9" src={imgPressure}/>
            <p>{reads.hum} %</p>
          </div>
          <p>Pressure</p>
        </div>

        <div className="flex flex-col items-center gap-2">
          <div className="flex items-center">
            <img className="w-9 h-9" src={imgAltitude} />
            <p>{reads.alt} m</p>
          </div>
          <p>Altitude</p>
        </div>
      </footer>
    </>
  );
}
