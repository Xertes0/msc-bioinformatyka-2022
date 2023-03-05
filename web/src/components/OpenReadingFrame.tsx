import React from "react";
import {
  OpenReadingFrameClick,
  OpenReadingFrameViewData,
} from "./AminoAcidView";

export default function OpenReadingFrame({
  color,
  data,
  index,
  onClick,
  selected,
}: {
  color: string;
  data: OpenReadingFrameViewData;
  index: [number, number];
  onClick(event: OpenReadingFrameClick): void;
  selected?: boolean;
}) {
  return (
    <tr
      key={index[0]}
      className={`orf${selected ? " active" : ""}`}
      onClick={(event) =>
        onClick(
          Object.assign(event, {
            target: event.target as HTMLTableRowElement,
            idx: index,
          }) as OpenReadingFrameClick
        )
      }
      data-shift={index[0]}
    >
      {data.shift > 0 && <td colSpan={data.shift * 3 + index[0]}></td>}
      {data.sequence.map((aa, aaIdx) => (
        <td
          key={`${index.join(":")}:${aaIdx}`}
          colSpan={3}
          className="aa"
          data-idx={aaIdx}
          style={{ borderColor: color }}
        >
          {aa}
        </td>
      ))}
    </tr>
  );
}
