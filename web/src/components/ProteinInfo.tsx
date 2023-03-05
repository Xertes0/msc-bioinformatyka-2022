import { AminoAcidData } from "../AminoAcidData";

const ProteinInfo = ({ seq }: { seq: (AminoAcidData | undefined)[] }) => (
  <div className="protein-info">
    <table>
      <thead>
        <tr>
          <th>name</th>
          <th>abbreviation</th>
          <th>symbol</th>
          <th>molecularWeight</th>
          <th>residueWeight</th>
          <th>molecularFormula</th>
          <th>residueFormula</th>
          <th>pKa</th>
          <th>pKb</th>
          <th>pKx</th>
          <th>pl</th>
        </tr>
      </thead>
      <tbody>
        {seq.map((aa, idx) => (
          <tr key={idx}>
            <td>{aa?.name || "-"}</td>
            <td>{aa?.abbreviation || "-"}</td>
            <td>{aa?.symbol || "-"}</td>
            <td>{aa?.molecularWeight || "-"}</td>
            <td>{aa?.residueWeight || "-"}</td>
            <td>{aa?.molecularFormula || "-"}</td>
            <td>{aa?.residueFormula || "-"}</td>
            <td>{aa?.pKa || "-"}</td>
            <td>{aa?.pKb || "-"}</td>
            <td>{aa?.pKx || "-"}</td>
            <td>{aa?.pl || "-"}</td>
          </tr>
        ))}
      </tbody>
    </table>
  </div>
);

export default ProteinInfo;
