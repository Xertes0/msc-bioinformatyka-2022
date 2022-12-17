import "./App.css";
import React, { useEffect, useState } from "react";

import OpenReadingFrame from "./components/OpenReadingFrame";
// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";

function App() {
    const [bioModule, setBioModule] = useState({ bio_translate: (_string: string, _callback: Function) => [] });
    const [bioModuleLoaded, setBioModuleLoaded] = useState(false);
    const [orfs, setOrfs] = useState([]);

    useEffect(() => {
        setBioModuleLoaded(false);
        BioModule().then((res: any) => {
            setBioModuleLoaded(true);
            setBioModule(res);
        })
    }, []);

    function orfClick(event: React.MouseEvent<HTMLDivElement>) {
        let element = event.target as HTMLDivElement;
        if (element.tagName != "A") {
            return;
        }
        window.open(`skeletal.html?formula=${element.innerText}`, '_blank', 'noopener noreferrer');
    }

    function transSubmit(event: React.FormEvent<HTMLFormElement>) {
        event.preventDefault();
        let form = event.target as HTMLFormElement;
        let input = form.children[0].children[1] as HTMLInputElement;
        let str = input.value;
        console.log(input);
        if (str == "") {
            return;
        }
        setOrfs(bioModule.bio_translate(
            str,
            (index: Number) => {
                return [`<a id='proteinSeq${index}'>`, "</a>"];
            }
        )
        );
    }

    return (
        <div className="App">
            <div className="card">
                <h1>Translate a sequence</h1>
                {
                    bioModuleLoaded &&
                    <div className="card">
                        <form onSubmit={transSubmit}>
                            <label>
                                Sequence<br></br>
                                <input type="text"></input>
                            </label>
                        </form>
                    </div>
                }
                <div onClick={orfClick}>
                    {
                        orfs.map((orf, index) => {
                            return <OpenReadingFrame key={index} id={index} content={orf} />
                        })
                    }
                </div>
            </div>
        </div>
    )
}

export default App;
