import ProgressRing from "./ProgressRing";

export default function Home({ imgDay, imgAlt, temp, alt, hum }) {
  return (
    <>
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
    </>
  );
}
