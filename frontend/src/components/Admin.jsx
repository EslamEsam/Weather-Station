import React, { useState } from "react";
import { initializeApp } from "firebase/app";
import { getDatabase, ref, onValue, set } from "firebase/database";

const firebaseConfig = {
  databaseURL: "https://iot-qamd-default-rtdb.firebaseio.com",
};

const app = initializeApp(firebaseConfig);
const db = getDatabase();

export default function Admin() {
  const [config, setConfig] = useState({
    email: "",
    tempThresh: "",
    presThresh: "",
    altThresh: "",
    humThresh: "",
  });

  const submitConfig = (e) => {
    e.preventDefault();

    console.log(config);

    for (const prop in config) {
      if (config[prop]) {
        set(ref(db, `config/${prop}`), config[prop]);
      }
    }

    // set(ref(db, "config"), {
    //   ...config,
    // });
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
            onChange={(e) =>
              setConfig((old) => ({ ...old, email: e.target.value }))
            }
            value={config.email}
          />
        </div>

        <div className="flex gap-4">
          <div className="mb-6 w-full">
            <label
              htmlFor="humThresh"
              className="block mb-2 text-sm font-medium text-gray-900"
            >
              Humidity Threshold
            </label>
            <input
              type="number"
              id="humThresh"
              className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
              onChange={(e) =>
                setConfig((old) => ({ ...old, humThresh: e.target.value }))
              }
              placeholder="35"
            />
          </div>

          <div className="mb-6 w-full">
            <label
              htmlFor="tempThresh"
              className="block mb-2 text-sm font-medium text-gray-900"
            >
              Tempreature Threshold
            </label>
            <input
              type="number"
              id="tempThresh"
              className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
              onChange={(e) =>
                setConfig((old) => ({ ...old, tempThresh: e.target.value }))
              }
              value={config.tempThresh}
              placeholder="15"
            />
          </div>
        </div>

        <div className="flex gap-4">
          <div className="mb-6 w-full">
            <label
              htmlFor="altThresh"
              className="block mb-2 text-sm font-medium text-gray-900"
            >
              Altitude Threshold
            </label>
            <input
              type="number"
              id="altThresh"
              className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
              onChange={(e) =>
                setConfig((old) => ({ ...old, altThresh: e.target.value }))
              }
              placeholder="35"
            />
          </div>

          <div className="mb-6 w-full">
            <label
              htmlFor="presThresh"
              className="block mb-2 text-sm font-medium text-gray-900"
            >
              Pressure Threshold
            </label>
            <input
              type="number"
              id="presThresh"
              className="bg-gray-50 border border-gray-300 text-gray-900 text-sm rounded-lg focus:ring-blue-500 focus:border-blue-500 block w-full p-2.5"
              onChange={(e) =>
                setConfig((old) => ({ ...old, presThresh: e.target.value }))
              }
              placeholder="35"
            />
          </div>
        </div>

        <button
          type="submit"
          className="block sm:w-full text-white bg-blue-700 hover:bg-blue-800 focus:ring-4 focus:outline-none focus:ring-blue-300 font-medium rounded-lg text-sm w-full px-5 py-2.5 text-center"
          onClick={(e) => submitConfig(e)}
        >
          Update
        </button>
      </form>
    </main>
  );
}
