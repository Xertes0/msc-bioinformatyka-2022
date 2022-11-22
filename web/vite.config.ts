import {defineConfig, searchForWorkspaceRoot} from 'vite'
import react from '@vitejs/plugin-react'

import CxxConfig from "./cmake-autogen.config";

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [react()],
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
  }
})
