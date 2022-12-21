import React, { useState } from "react";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, set } from "firebase/database";

const firebaseConfig = {
  databaseURL: "https://iot-qamd-default-rtdb.firebaseio.com",
};
const app = initializeApp(firebaseConfig);
const db = getDatabase(app);

export default function Admin() {
  const [email, setEmail] = useState("");
  const [thresh, setThresh] = useState(0);

  const submitConfig = (e) => {
    e.preventDefault();

    set(ref(db, "config"), {
      thresh: thresh,
    });

    set(ref(db, "config_email"), {
      email: email,
    });
  };

  return (
    <main className="w-[450px]">
      <h1 className="mb-6 text-3xl">Admin Panel</h1>

      <form>
        <div className="mb-6">
          <label
            htmlFor="email"
            className="block mb-2 text-sm font-medium text-gray-900"
          >
            Email Address
          </label>
          <input
            type="text"
            id="email"
            className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
            placeholder="qamd.offff@gamil.com"
            onChange={(e) => setEmail(e.target.value)}
            value={email}
          />
        </div>
        <div className="mb-6">
          <label
            htmlFor="thresh"
            className="block mb-2 text-sm font-medium text-gray-900"
          >
            Threshold
          </label>
          <input
            type="number"
            id="thresh"
            className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
            onChange={(e) => setThresh(e.target.value)}
            placeholder="15"
          />
        </div>

        <button
          type="submit"
          className="text-white bg-blue-700 hover:bg-blue-800 focus:ring-4 focus:outline-none focus:ring-blue-300 font-medium rounded-lg text-sm w-full sm:w-auto px-5 py-2.5 text-center"
          onClick={(e) => submitConfig(e)}
        >
          Update
        </button>
      </form>
    </main>
  );
}
