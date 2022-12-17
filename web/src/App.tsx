import React, {useEffect, useState} from "react";
import "./App.css";
import Orf from "./components/Orf";

// @ts-ignore
import BioModule from "@cxx/biolib/bio.mjs";

function App() {
    const [bioModule, setBioModule] = useState({bio_translate: (string: string, callback: Function) => []});
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
        let input = form.children[0] as HTMLInputElement;
        let str = input.value;
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
        // document.getElementById("orf0").innerHTML = orfs[0];
        // document.getElementById("orf1").innerHTML = orfs[1];
        // document.getElementById("orf2").innerHTML = orfs[2];
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
                                return <Orf id={index} content={orf}/>
                            }
                        )
                    }
                    {/*<div id="orf0" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>*/}
                    {/*</div>*/}
                    {/*<div id="orf1" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>*/}
                    {/*</div>*/}
                    {/*<div id="orf2" className="card" style={{border: "1px solid grey", marginBottom: "2px"}}>*/}
                    {/*</div>*/}
                </div>
            </div>
        </div>
    )
}

export default App;
