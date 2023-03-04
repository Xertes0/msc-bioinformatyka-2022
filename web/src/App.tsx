import "./App.css";
import React, { useState } from "react";

import SequenceInput from "./SequenceInput";
import ProteinView from "./ProteinView";

function App() {
    const [sequence, setSequence] = useState("");

    function submit(sequence: string) {
        setSequence(sequence);
    }

    return (
        <div className="App">
            <div className="card">
                <h1>Input a nucleic acid sequence:</h1>
                <div className="form-container">
                    <SequenceInput onSubmit={submit} />
                    <div className="form-description-container">
                        <h2>The input accepts nucleobases mapped as follows:</h2>
                        <ul>
                            <li>A = Adenine</li>
                            <li>C = Cytosine</li>
                            <li>G = Guanine</li>
                            <li>T = Thymine</li>
                            <li>U = Uracil</li>
                        </ul>
                    </div>
                </div>
                { /* sequence  && */ <ProteinView sequence={sequence} onProteinClick={() => {  }}/> }
            </div>
        </div>
    )
}

export default App;
