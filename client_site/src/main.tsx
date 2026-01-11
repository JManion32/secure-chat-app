// src/main.tsx
import { createRoot } from "react-dom/client";
import App from "./App";

import "./css/index.css";
import "./css/colors.css";
import "./css/scaling.css";

createRoot(document.getElementById("root")!).render(
  <App />
);
