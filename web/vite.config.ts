import {defineConfig, searchForWorkspaceRoot} from 'vite'
import react from '@vitejs/plugin-react'
import {resolve} from 'path'

import CxxConfig from "./cmake-autogen.config";

// https://vitejs.dev/config/
export default defineConfig({
    base: "/msc-bioinformatyka-2022/",
    plugins: [
        react()
    ],
    resolve: {
        alias: CxxConfig.aliases,
        extensions: ['.mjs', '.js', '.ts', '.jsx', '.tsx', '.json', '.wasm']
    },
    server: {
        fs: {
            allow: [
                searchForWorkspaceRoot(process.cwd())
            ].concat(CxxConfig.fsAllow)
        }
    },
    build: {
        rollupOptions: {
            input: {
                main: resolve(__dirname, 'index.html'),
                skeletal: resolve(__dirname, 'skeletal.html')
            }
        }
    }
})
