import Progress from "react-circle-progress-bar";
import React from "react";
const ProgressRing = ({ className, percentage = 0, style = [] }) => {
  const [
    width = 60,
    height = 60,
    color = "#27b5dc",
    trailColor = "#dddccc",
    textColor = "#fff",
    textSize = "90%",
    strokeWidth = 12,
  ] = style;
  return (
    <div className="relative">
      {" "}
      <Progress
        className={`${className} rotate-180`}
        style={{ width: width, height: height }}
        progress={percentage}
        strokeWidth={strokeWidth}
        background={trailColor}
        gradient={[
          { stop: 0.0, color: color },
          { stop: 1, color: color },
        ]}
        reduction={0}
        hideBall
        hideValue
      />{" "}
      <span
        className={`absolute -translate-x-1/2 -translate-y-1/2 top-1/2 left-1/2 text-[${textColor}`}
        style={{ fontSize: textSize }}
      >
        {" "}
        {percentage}%{" "}
      </span>{" "}
    </div>
  );
};
export default ProgressRing;
