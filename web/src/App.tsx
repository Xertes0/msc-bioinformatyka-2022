import {useEffect, useState} from 'react';
import reactLogo from './assets/react.svg';
import './App.css';

// @ts-ignore
import BioModule from '@cxx/biolib/bio.mjs';

function App() {
    const [bioModule, setBioModule] = useState({
        bio_test() {
            return Number();
        }
    });
    const [bioModuleLoaded, setBioModuleLoaded] = useState(false);

    useEffect(() => {
        setBioModuleLoaded(false);
        BioModule().then((res: any) => {
            setBioModuleLoaded(true);
            setBioModule(res);
        })
    }, []);

    return (
        <div className="App">
            <div>
                <a href="https://vitejs.dev" target="_blank">
                    <img src="/vite.svg" className="logo" alt="Vite logo"/>
                </a>
                <a href="https://reactjs.org" target="_blank">
                    <img src={reactLogo} className="logo react" alt="React logo"/>
                </a>
            </div>
            <h1>Vite + React</h1>
            <div className="card">
                <p>
                    {
                        bioModuleLoaded ? bioModule.bio_test() : 'Mada mada desu'
                    }
                </p>
            </div>
            <p className="read-the-docs">
                Click on the Vite and React logos to learn more
            </p>
        </div>
    )
}

export default App;
